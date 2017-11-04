// sensors.c in /Blackbird
// Contains subroutines for obtaining sensor readings

// Sensor related includes
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <StellarisWare/driverlib/rom.h>




// LoopDelay
// Uses ROM_SysCtlDelay to set up a loop delay in a clean and robust manner
// time is in seconds
void LoopDelay(float time) {
    ROM_SysCtlDelay(ROM_SysCtlClockGet() * time / 3);
}

// Pulse
// Sends a 15us pulse to trigger ultrasonic distance sensors
void PulsePin(tPin pin) {

    // Computation of clock cycles necessary for 15us(+/-20ns) pulse
    // 80 clocks is the approximate execution time of SetPin
    // ROM_SysCtlClockGet is much more consistent than SysCtlClockGet
    unsigned long ulSysPeriod = ROM_SysCtlClockGet();
    int delay = (int) (ulSysPeriod*0.000005 - 80);

    // Pulse pin by 15us
    SetPin(pin, false);
    ROM_SysCtlDelay(delay);
    SetPin(pin, true);
}


// Basic math for getDistance
float* calculateDistance(long start_time_1, long end_time_1, long start_time_2, long end_time_2) {

  static float output[2];
  
  // Pulse width
  long time_1 = end_time_1 - start_time_1;
  long time_2 = end_time_2 - start_time_2;

  // Compute distance
  // t = 2d/v => d = tv/2 = 34000*t(us)/(1000000*2) = 0.0017t(us) cm
  output[0] = (float) time_1 * 0.017;
  output[1] = (float) time_2 * 0.017;

  return(output);
}


// The most interesting hardware-related function
// Pings each distance sensor pair in sequence, and computes each distance
float * getDistance(long timeout_us,tPin * trigger_pins,tPin echo_1, tPin echo_2){
  

  // Output array for each loop.
  // Arranged CCW, with sensor 0 on the right and sensor 10 on the left.
  static float distances[10];
  
  // Pass through loop for every trigger pair
  for(int i=0; i<5; i++) {

    // Initialize variables for the loop
    long start_time_1 = 0;
    long start_time_2 = 0;
    long end_time_1 = 0;
    long end_time_2 = 0;
    long timeout_time = (long) GetTimeUS() + timeout_us;
  
    // Pulse pin by 15us
    PulsePin(trigger_pins[i]);
    
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

    float *  distance_pair = calculateDistance(start_time_1, end_time_1, start_time_2, end_time_2);

    // Save Information
    // The trigger pair consists of two sensors 90 degrees from each other
    distances[i] = distance_pair[0];
    distances[i+5] = distance_pair[1];

  }
  // Returns
  return distances;
}
