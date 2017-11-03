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
#include <RASLib/inc/motor.h>

//
//
// Global Constants
//
//

tMotor *left;
tMotor *right;

tBoolean led_on;

// How long should we wait for the distance sensors?
long timeout_us = 15000;

// Array containing the 5 trigger pair pin mappings
tPin trigger_pins[5] = {PIN_D0,PIN_D1,PIN_D2,PIN_D3,PIN_E1};

// Two echo pins
tPin echo_1 = PIN_E2;
tPin echo_2 = PIN_E3;

// Global diagnostic tracking

int iteration = 0;
int time_faults_cumulative = 0;



//
//
//  Simple Functions
//
//

// Printf from uart.h doesn't support long. Here we go!
// The number is cast to long since the fucking launchpad doesn't seem to support the division of long long int; (long long int) x / n or x % n gives make errors, but +,-,* are fine.
void GhettoPrintf(char *message,long number){
  Printf("%s: %d.%d\n", message, (int) (number / 10000), (int) (number % 10000));
}


// Initializes servos. LM4Fs have issues with motor directions and speeds. Fortunately, TM4Cs work fine.
// THIS WILL NOT WORK ON A LM4F
void initServos(void){
  left = InitializeServoMotor(PIN_E4, false);
  right = InitializeServoMotor(PIN_E5, true);
}



//
//
// Complicated Functions
//
//


// The most interesting hardware-related function
// Pings each distance sensor pair in sequence, and computes each distance
float * pulsePin(void){

	
  // Diagnostic Info
  int time_faults = 0;
  

  // Output array for each loop.
  // Arranged CCW, with sensor 0 on the right and sensor 10 on the left.
  static float distances[10];
  
  // Pass through loop for every trigger pair
  for(int i=0; i<5; i++) {
    
    // Computation of clock cycles necessary for 15us(+/-20ns) pulse
    // 80 clocks is the approximate execution time of SetPin
    // ROM_SysCtlClockGet is much more consistent than SysCtlClockGet
    unsigned long ulSysPeriod = ROM_SysCtlClockGet();
    int delay = (int) (ulSysPeriod*0.000005 - 80);

    // Initialize variables for the loop
    long start_time_1 = 0;
    long start_time_2 = 0;
    long end_time_1 = 0;
    long end_time_2 = 0;
    long timeout_time = (long) GetTimeUS() + timeout_us;
    long time_remaining_previous = timeout_us;
  
    // Pulse pin by 15us
    SetPin(trigger_pins[i], false);
    ROM_SysCtlDelay(delay);
    SetPin(trigger_pins[i], true);

    // Sensor loop
    // while (true) is used with break to minimize the chance of launchpad fuckery
    while ( true) {

      // Compute Time
      long time_remaining = timeout_time - (long) GetTimeUS();

      // End condition: timeout (error states)
      if( time_remaining <= 0) {
	
	// Error state: no ping received (too close)
	// Error = -17000
	if(start_time_1 == 0) {
	  start_time_1 = 1000000;
	  end_time_1 = 0;
	}

	// Error state: no ping received (too far) -- timeout
	// Error = 17000
      	else if(end_time_1 == 0)
      	{
      		end_time_1 = start_time_1 + timeout_us;
      	}
	
	// Same for time 2
	if(start_time_2 == 0) {
	  start_time_2 = 1000000;
	  end_time_2 = 0;
	}
      	else if(end_time_2 == 0)
      	{
      		end_time_2 = start_time_2 + timeout_us;
      	}
	break;
      }

      if( end_time_1 != 0 && end_time_2 != 0) {
      	break;
      }

      // Defense against time travelling GetTimeUS()
      if( time_remaining_previous < time_remaining) {
	time_faults += 1;
	Printf("Time fault\n");
	break;
      }
      time_remaining_previous = time_remaining;


      // Sets start times and end times when appropriate
      if (GetPin(echo_1) == false && start_time_1 != 0 && end_time_1 == 0) {
	end_time_1 = (long) GetTimeUS();
      }
      if (GetPin(echo_2) == false && start_time_2 != 0 && end_time_2 == 0) {
	end_time_2 = (long) GetTimeUS();
      }   
      if (GetPin(echo_1) == true && start_time_1 == 0) {
	start_time_1 = (long) GetTimeUS();
      }
      if (GetPin(echo_2) == true && start_time_2 == 0) {
	start_time_2 = (long) GetTimeUS();
      }
    }

    // Compute pulse width
    long time_1 = end_time_1 - start_time_1;
    long time_2 = end_time_2 - start_time_2;

    // Compute distance
    // t = 2d/v => d = tv/2 = 34000*t(us)/(1000000*2) = 0.0017t(us) cm
    float distance_1 = (float) time_1 * 0.017;
    float distance_2 = (float) time_2 * 0.017;

    // Save Information
    // The trigger pair consists of two sensors 90 degrees from each other
    distances[i] = distance_1;
    distances[i+5] = distance_2;
    
    // This one goes in a global.
    // It's a global because we're fighting jank with jank
    time_faults_cumulative += time_faults;

  }
  // Returns
  return distances;
}


int main(void){
  
  Printf("Hello World\n\n");
  
  InitializeGPIO();
  initServos();

  while(true) {

    // Diagnostic info
    long loop_start_time = (long) GetTimeUS();

    // Heartbeat LED
    SetPin(PIN_F1, led_on);
    SetPin(PIN_F3, !led_on);
    led_on = !led_on;


    // The actual function
    float * distances = pulsePin();

    // Time computation
    long execution_time = (long) GetTimeUS() - loop_start_time;
    
    // Timestamp update
    iteration += 1;    
  
    // Print out info
    Printf("Iteration: %d\n",iteration);
    GhettoPrintf("Execution time (us)",execution_time);
    
    // Uncomment for the juiciest gossip about time travel
    // (or the lack thereof)
    /*
      Printf("Cumulative time faults: %d\n", time_faults_cumulative);
      Printf("Time faults: %d\n",time_faults);
    */
    

    for(int i=0;i < 10; i++) {
      Printf("Distance %d: %f cm\n",i,*(distances+i));
    }
  
    // Loop Delay
    ROM_SysCtlDelay(ROM_SysCtlClockGet()/15);
  }
  return 0;
}
