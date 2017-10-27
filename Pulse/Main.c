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
long timeout_us = 15000;

// Array containing the 5 trigger pair pin mappings
tPin trigger_pins[5] = {PIN_A2,PIN_A3,PIN_A4,PIN_B6,PIN_B7};

// Two echo pins
tPin echo_1 = PIN_D6;
tPin echo_2 = PIN_D7;

// Loop time variables
long start_time_1;
long start_time_2;
long end_time_1;
long end_time_2;
long timeout_time;
long time_remaining_previous;
  



// Printf from uart.h doesn't support long. Here we go!
// The number is cast to long since the fucking launchpad doesn't seem to support the division of long long int; (long long int) x / n or x % n gives make errors, but +,-,* are fine.
void GhettoPrintf(char *message,long number){
  Printf("%s: %d.%d\n", message, (int) (number / 10000), (int) (number % 10000));
}

// Timestamp info and Diagnostics
int iteration = 0;
int time_faults_cumulative = 0;



// Where everything happens
void pulsePin(void){

	
  // Diagnostic Info
  GhettoPrintf("gettime", (long) GetTimeUS());
  long loop_start_time = (long) GetTimeUS();
  int time_faults = 0;
  
  // Heartbeat LED
  SetPin(PIN_F1, led_on);
  SetPin(PIN_F3, !led_on);
  led_on = !led_on;

  // Output array for each loop.
  // Arranged CCW, with sensor 0 on the right and sensor 10 on the left.
  float distances[10];
  int times[10];
  
  // Pass through loop for every trigger pair
  for(int i=0; i<5; i++) {
    
    // Computation of clock cycles necessary for 15us(+/-20ns) pulse
    // 80 clocks is the approximate execution time of SetPin
    // ROM_SysCtlClockGet is much more consistent than SysCtlClockGet
    unsigned long ulSysPeriod = ROM_SysCtlClockGet();
    int delay = (int) (ulSysPeriod*0.000005 - 80);

    // Initialize variables for the loop
    start_time_1 = 0;
    start_time_2 = 0;
    end_time_1 = 0;
    end_time_2 = 0;
    timeout_time = (long) GetTimeUS() + timeout_us;
    time_remaining_previous = timeout_us;
  
    // Pulse pin by 15us
    SetPin(trigger_pins[i], false);
    ROM_SysCtlDelay(delay);
    SetPin(trigger_pins[i], true);

    // Sensor loop
    // while (true) is used with break to minimize the chance of launchpad fuckery
    while ( true) {

      // Compute Time
      long time_remaining = timeout_time - (long) GetTimeUS();

      // End condition
      if( time_remaining <= 0) {
      	Printf("Timeout\n");
	break;
      }

      // Defense against time travelling GetTimeUS()
      // Generally speaking, GetTimeUS() will time travel twice on boot
      if( time_remaining_previous < time_remaining) {
	time_faults += 1;
	Printf("Time fault\n");
	break;
      }
      time_remaining_previous = time_remaining;

      // Record the pulse start and end times.
      Printf("echoes:%d|%d", GetPin(echo_1),GetPin(echo_2));

      if (GetPin(echo_1) == true && start_time_1 == 0) {
	start_time_1 = (long) GetTimeUS();
	Printf("Triggered1on\n");
      }
      if (GetPin(echo_2) == true && start_time_2 == 0) {
	start_time_2 = (long) GetTimeUS();
	Printf("Triggered2on\n");
      }
      if (GetPin(echo_1) == false && start_time_1 != 0 && end_time_1 == 0) {
	end_time_1 = (long) GetTimeUS();
      }
      if (GetPin(echo_2) == false && start_time_2 != 0 && end_time_2 == 0) {
	end_time_2 = (long) GetTimeUS();
      }   
    }

    // Compute pulse width
    // (int) is used since max pulse length is ~22000us
    int time_1 = (int) (end_time_1 - start_time_1);
    int time_2 = (int) (end_time_2 - start_time_2);

    // Compute distance
    // t = 2d/v => d = tv/2 = 34000*t(us)/(1000000*2) = 0.0017t(us) cm
    float distance_1 = (float) time_1 * 0.0017;
    float distance_2 = (float) time_2 * 0.0017;

    // Save Information
    // The trigger pair consists of two sensors 90 degrees from each other
    distances[i] = distance_1;
    distances[i+5] = distance_2;
    times[i] = time_1;
    times[i+5] = time_2;
    
    // Print out info
    
    /*Printf("Time 1: %d us\n", time_1);
    Printf("Time 2: %d us\n", time_2);
    Printf("Distance 1: %f cm\n", distance_1);
    Printf("Distance 2: %f cm\n", distance_2);
    Printf("\n");*/
    
  }

  // Time computation
  long execution_time = (long) GetTimeUS() - loop_start_time;
  GhettoPrintf("gettime", (long) GetTimeUS());
  
  // Print out info
  Printf("Iteration: %d\n",iteration);
  Printf("Cumulative time faults: %d\n", time_faults_cumulative);
  GhettoPrintf("Execution time (us)",execution_time);
  Printf("Time faults: %d\n",time_faults);
  time_faults_cumulative += time_faults;

  
  for(int i=0;i < 10; i++) {
    Printf("Distance %d: %f cm\n",i,distances[i]);
    Printf("Time %d: %d us\n",i,times[i]);
  }
  Printf("\n");
  
  
  // Timestamp update
  iteration += 1;
  
}

int main(void){
	for(int i = 0; i < 5; i++)
	{
		SetPin(trigger_pins[i], true);
	}
  
  Printf("Hello World\n\n");
  InitializeGPIO();
  //CallEveryUS(pulsePin, 0, 60000);
  //CallEvery(pulsePin, 0, 2);
  while(true) {
  	pulsePin();
  	ROM_SysCtlDelay(ROM_SysCtlClockGet());
  }

  return 0;

}
