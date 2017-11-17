// sensors.c in /Blackbird
// Contains subroutines for obtaining sensor readings

#include "sensors.h"



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
  // t = 2d/v => d = tv/2 = 34300*t(us)/(1000000*2) = 0.01715t(us) cm
  output[0] = (float) time_1 * 0.01715;
  output[1] = (float) time_2 * 0.01715;

  return(output);
}


// The most interesting hardware-related function
// Pings each distance sensor pair in sequence, and computes each distance
// Returns the execution time
long getDistance(pointSet * points, HWProfile * profile, timeTracker * tracker){

  // Will be used at the end to get the loop time
  long loop_start_time = (long) GetTimeUS();
  
  // Pass through loop for every trigger pair
  for(int i=0; i<5; i++) {

    // Initialize variables for the loop
    long start_time_1 = 0;
    long start_time_2 = 0;
    long end_time_1 = 0;
    long end_time_2 = 0;
    long timeout_time = (long) GetTimeUS() + profile->timeout_us;

    // Pulse pin by 15us
    PulsePin((profile->trigger_pins)[i]);
    
    // Sensor loop
    // while (true) is used with break to minimize the chance of launchpad fuckery
    while ( true) {

      // Compute Time
      long time_remaining = timeout_time - (long) GetTimeUS();

      // End condition: timeout (error states)
      if( time_remaining <= 0) {
	
	// Error state: device disconnected
	// Error = -17000
	if(start_time_1 == 0) {
	  start_time_1 = 1000000;
	  end_time_1 = 0;
	}

	// Error state: no ping received (too far) -- timeout
	// Error = 17000
      	else if(end_time_1 == 0)
      	{
      		end_time_1 = start_time_1 + profile->timeout_us;
      	}
	
	// Same for time 2
	if(start_time_2 == 0) {
	  start_time_2 = 1000000;
	  end_time_2 = 0;
	}
      	else if(end_time_2 == 0)
      	{
      		end_time_2 = start_time_2 + profile->timeout_us;
      	}
	break;
      }

      // Both pings received
      /* if( end_time_1 != 0 && end_time_2 != 0) { */
      /* 	break; */
      /* } */


      // Sets start times and end times when appropriate
      if (GetPin(profile->echo_1) == false && start_time_1 != 0 && end_time_1 == 0) {
	end_time_1 = (long) GetTimeUS();
      }
      if (GetPin(profile->echo_2) == false && start_time_2 != 0 && end_time_2 == 0) {
	end_time_2 = (long) GetTimeUS();
      }   
      if (GetPin(profile->echo_1) == true && start_time_1 == 0) {
	start_time_1 = (long) GetTimeUS();
      }
      if (GetPin(profile->echo_2) == true && start_time_2 == 0) {
	start_time_2 = (long) GetTimeUS();
      }
    }
    float *  distance_pair = calculateDistance(start_time_1, end_time_1, start_time_2, end_time_2);
    
    // Save Information
    // Information is saved to a pointSet struct
    // The trigger pair consists of two sensors 90 degrees from each other
    //(points->r)[i] = ((points->r)[i] + distance_pair[0])/2;
    //(points->r)[i+5] = ((points->r)[i+1] + distance_pair[1])/2;
    (points->r)[i] = distance_pair[0];
    (points->r)[i+5] = distance_pair[1];
  }

  // Return the total function time
  long etime = (long) GetTimeUS() - loop_start_time;
  tracker->execution_time = etime;
  return (etime);
  
}

// Get line sensor output, and apply basic filtering
void getLineData(pointSet * points, HWProfile * profile) {

 

  for (int i=0; i<5; i++) {

    (points->reflectances)[i] = ADCRead((profile->linesensor)[i]);
   
    // if the sensor is more reflective than profile.threshhold
    // assign white (0) to that sensor    
    if ((points->reflectances)[i] < profile->threshhold) {
      (points->line)[i] = '0';
    }

    // else, assign black
    else {
      (points->line)[i] = '1';
    }
  }
}
