#ifndef _DISPLAY
#define _DISPLAY

#include <RASLib/inc/common.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/uart.h>

#include "structures.h"
#include "raspy.h"

// display.h in /Blackbird
// header file for display related subroutines

// Printf from uart.h doesn't support long. Here we go!
// The number is cast to long since the fucking launchpad doesn't seem to support the division of long long int; (long long int) x / n or x % n gives make errors, but +,-,* are fine.
void GhettoPrintf(char *message,long number);

// Print out each distance from a *array
// 10 elements corresponding with the 10 distance sensors
void PrintOutDistances(pointSet * points);

// Print out line sensor array
// 8 elements corresponding with 8 infrared phototransistors
// 1 indicates presence of a black line; 0 indicates no line
void PrintOutLine(pointSet * points);

// Updates the timeTracker
void updateTimeTracker(timeTracker * tracker);

// Print out timeTracker
void printTimeTracker(timeTracker * tracker);

// Make the LED display an error color code
// Will flash a different color every cycle:
// Green = normal operation
// Blue = disconnected distance sensor
void ledColorError(pointSet * points);

// Prints out info
// if profile->mode = python-mode, print out to pyserial
// if profile->mode = uart-mode, print out in a human-readable form
void printOutInfo(pointSet * points, HWProfile * profile, timeTracker * time);

#endif
