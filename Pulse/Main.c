// Main.c in /Pulse
// Shoots ultrasonic sensors in all directions
// Gets the return and prints out the distance

#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/uart.h>
#include <stdio.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/rom.h>
#include <StellarisWare/driverlib/timer.h>
#include <StellarisWare/driverlib/systick.h>

tBoolean led_on;

// How long should we wait for the distance sensors?
tTime timeout_us = 10000;

// Array containing the 5 trigger pair pin mappings
tPin trigger_pins[5] = {PIN_A2,PIN_A3,PIN_A4,PIN_B6,PIN_B7};

// Two echo pins
tPin echo_1 = PIN_C6;
tPin echo_2 = PIN_C7;

// Printf from uart.h doesn't support long. Here we go!
// The number is cast to long since the fucking launchpad doesn't seem to support the division of long long int; (long long int) x / n or x % n gives make errors, but +,-,* are fine.
void GhettoPrintf(char *message,tTime number){
  Printf("%s: %d.%d\n", message, (int) ((long)number / 10000), (int) ((long)number % 10000));
}

// Where everything happens
void pulsePin(void){

  // Diagnostic Info
  unsigned long loop_start_time = GetTimeUS();
  int time_faults = 0;
  
  // Heartbeat LED
  SetPin(PIN_F1, led_on);
  SetPin(PIN_F3, !led_on);
  led_on = !led_on;

  // Output array for each loop.
  // Arranged CCW, with sensor 0 on the right and sensor 10 on the left.
  float distances[10];
  
  // Pass through loop for every trigger pair
  for(int i=0; i<5; i++) {
    
    // Computation of clock cycles necessary for 15us(+/-20ns) pulse
    // 80 clocks is the approximate execution time of SetPin
    // ROM_SysCtlClockGet is much more consistent than SysCtlClockGet
    unsigned long ulSysPeriod = ROM_SysCtlClockGet();
    int delay = (int) (ulSysPeriod*0.000005 - 80);

    // Initialize variables for the loop
    tTime start_time_1 = 0;
    tTime start_time_2 = 0;
    tTime end_time_1 = 0;
    tTime end_time_2 = 0;
    tTime start_time = GetTimeUS();
    tTime timeout_time = start_time + timeout_us;
    long time_remaining_previous = timeout_time;
  
    // Pulse pin by 15us
    SetPin(trigger_pins[i], true);
    ROM_SysCtlDelay(delay);
    SetPin(trigger_pins[i], false);

    // Sensor loop
    // while (true) is used with break to minimize the chance of launchpad fuckery
    while ( true) {

      // Compute Time
      long current_time = (long) GetTimeUS();
      long time_remaining = timeout_time - current_time;

      // Defense against time travelling GetTimeUS()
      // Generally speaking, GetTimeUS() will time travel twice on boot
      if( time_remaining_previous < time_remaining) {
	GhettoPrintf("Time fault",time_remaining);
	time_faults += 1;
	break;
      }
      time_remaining_previous = time_remaining;

      // End condition
      if( time_remaining <= 0) {
	break;
      }

      // Record the pulse start and end times.
      if (GetPin(echo_1) == true && start_time_1 == 0) {
	start_time_1 = GetTimeUS();
      }
      if (GetPin(echo_2) == true && start_time_2 == 0) {
	start_time_2 = GetTimeUS();
      }
      if (GetPin(echo_1) == false && start_time_1 != 0) {
	end_time_1 = GetTimeUS();
      }
      if (GetPin(echo_2) == false && start_time_2 != 0) {
	end_time_2 = GetTimeUS();
      }   
    }

    // Compute pulse width
    // (int) is used since max pulse length is ~22000us
    int time_1 = (int) end_time_1 - start_time_1;
    int time_2 = (int) end_time_2 - start_time_2;

    // Compute distance
    // t = 2d/v => d = tv/2 = 34000*t(us)/(1000000*2) = 0.017t(us) cm
    float distance_1 = (float) time_1 * 0.017000;
    float distance_2 = (float) time_2 * 0.017000;

    // Save Information
    // The trigger pair consists of two sensors 90 degrees from each other
    distances[i] = distance_1;
    distances[i+5] = distance_2;
    
    // Print out info
    Printf("Time 1: %d us\n", time_1);
    Printf("Time 2: %d us\n", time_2);
    Printf("Distance 1: %f cm\n", distance_1);
    Printf("Distance 2: %f cm\n", distance_2);
    Printf("\n");
  }

  // Time computation
  unsigned long execution_time = (long) GetTimeUS() - loop_start_time;
  
  // Print out info
  GhettoPrintf("Execution time",execution_time);
  if(time_faults > 0) {
    Printf("Time faults: %d\n",time_faults);
  }
  for(int i=0;i < 10; i++) {
    Printf("Distance %d:%d cm\n",i,distances[i]);
  }
}

int main(void){
  
  Printf("Hello World\n\n");
  CallEvery(pulsePin, 0, 1);

  return 0;

}
