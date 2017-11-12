#ifndef _SENSORS
#define _SENSORS

// sensors.h in /Blackbird
// Header file for sensor data related subroutines

#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>
#include <StellarisWare/driverlib/rom.h>

#include "structures.h"

// Uses ROM_SysCtlDelay to set up a loop delay in a clean and robust manner
// time is in seconds
void LoopDelay(float time);

// Sends a 15us pulse to trigger ultrasonic distance sensors
void PulsePin(tPin pin);

// Basic math for getDistance
float * calculateDistance(long start_time_1, long end_time_1, long start_time_2, long end_time_2);

// The most interesting hardware-related function
// Pings each distance sensor pair in sequence, and computes each distance
// Returns the execution time
long getDistance(pointSet * points, HWProfile * profile, timeTracker *tracker);

// not yet implemented
int checkRamp();

// Get line sensor output, and apply basic filtering
void getLineData(pointSet * points,HWProfile * profile);

#endif
