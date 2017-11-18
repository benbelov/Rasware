// display.c in /Blackbird
// File containing I/O, debugging, and diagnostic subroutines

#include "display.h"


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
    Printf("Obstacle number: %d\n",(points->obstacleIndex)[i]);
  }
}


// Print out line sensor array
// 8 elements corresponding with 8 infrared phototransistors
// 1 indicates presence of a black line; 0 indicates no line
void PrintOutLine(pointSet * points) {

  char line_array_str[16];
  for(int i=0;i < 8; i++) {
    line_array_str[2*i] = ' ';
    line_array_str[2*i+1] = (points->line)[i];
  }
  Printf("Line sensor: %s\n",line_array_str);
}


// Updates the timeTracker
void updateTimeTracker(timeTracker * tracker) {
  tracker->iteration += 1;
  (tracker->FPS)[tracker->FPS_pointer] = (float) 50000/(tracker->execution_time);
  tracker->FPS_pointer += 1;
  if(tracker->FPS_pointer >= 20) {
    tracker->FPS_pointer = 0;
  }
  tracker->avgFPS = 0;
  for(int i=0; i<20; i++) {
    tracker->avgFPS += (tracker->FPS)[i];
  }
}

// Print out timeTracker
void printTimeTracker(timeTracker * tracker) {
  Printf("Iteration: %d\n",tracker->iteration);
  GhettoPrintf("Execution time (us)",tracker->execution_time);
}

// Make the LED display an error color code
// Will flash a different color every cycle:
// Green = normal operation
// Blue = disconnected distance sensor
void ledColorError(pointSet * points) {

  points->errorCode = 10;

  // update the currentStatusCode
  points->currentStatusCode += 1;
  points->currentStatusCode %= 3;

  // If the current status is 'hearbeat':
  if (points->currentStatusCode == 0) {
    SetPin(PIN_F1, false); // red
    SetPin(PIN_F2, false); // blue
    SetPin(PIN_F3, true); // green
  }

  // If the current status is 'errorcode':
  else if (points->currentStatusCode == 1) {
    for (int i=0;i<10;i++) {
      // distance sensor disconnected; code = 1 (red)
      if ((points->r)[i] < 0) {
	points->errorCode = 1;
      }

      // distance sensor timeout; code = 2 (purple)
      else if ((points->r)[i] >= 257.25 && points->errorCode > 2) {
	points->errorCode = 2;
      }

      // distance sensor too close (in inaccurate range); code = 3 (blue)
      else if ((points->r)[i] <= 5 && points->errorCode > 3) {
	points->errorCode = 3;
      }
    }

    // Display color codes
    if (points->errorCode == 10) {
      SetPin(PIN_F1, false);
      SetPin(PIN_F2, false);
      SetPin(PIN_F3, false);
    } 
    else if (points->errorCode == 1) {
      SetPin(PIN_F1, true);
      SetPin(PIN_F2, false);
      SetPin(PIN_F3, false);
    }
    else if (points->errorCode == 2) {
      SetPin(PIN_F1, true);
      SetPin(PIN_F2, true);
      SetPin(PIN_F3, false);
    }
    else if (points->errorCode == 3) {
      SetPin(PIN_F1, false);
      SetPin(PIN_F2, true);
      SetPin(PIN_F3, false);
    }
    else {
      SetPin(PIN_F1, true);
      SetPin(PIN_F2, true);
      SetPin(PIN_F3, true);
    }
  }

  // If the current status is 'outputmode':
  // Other modes not yet implemented
  else if (points->currentStatusCode == 0) {
    if (points->executionMode == 0) {
      SetPin(PIN_F1, false);
      SetPin(PIN_F2, true);
      SetPin(PIN_F3, true);
    }
    else if (points->executionMode == 1) {
      SetPin(PIN_F1, true);
      SetPin(PIN_F2, false);
      SetPin(PIN_F3, true);
    }
  }
}

// Prints out info
// if profile->mode = python-mode, print out to pyserial
// if profile->mode = uart-mode, print out in a human-readable form
void printOutInfo (pointSet * points,HWProfile * profile,timeTracker * time, pidProfile * pid) {

  if (points->executionMode == 0) {
    printTimeTracker(time);
    PrintOutDistances(points);
    PrintOutLine(points);
  }
  
  //else if (points->executionMode == 1) {
  else {
    printToPySerial(points,profile,time, pid);
  }
}
