// display.c in /Blackbird
// File containing I/O, debugging, and diagnostic subroutines

#include "main.h"


// Printf from uart.h doesn't support long. Here we go!
// The number is cast to long since the fucking launchpad doesn't seem to support the division of long long int; (long long int) x / n or x % n gives make errors, but +,-,* are fine.
void GhettoPrintf(char *message,long number){
    Printf("%s: %d.%d\n", message, (int) (number / 10000), (int) (number % 10000));
}


// Print out each distance from a *array
// 10 elements corresponding with the 10 distance sensors
void PrintOutDistances(pointSet * points){
    for(int i=0;i < 10; i++) {
      Printf("Distance %d: %f cm\n",i,(points->r)[i]);
    }
}


// Make the LED display an error color code
// Will flash a different color every cycle:
// Green = normal operation
// Blue = disconnected distance sensor
void ledColorError(pointSet * points) {
}
