// Main.c in /Blackbird
// Goes ultrasonic
// Navigates around blocks

// All subroutines are in other files
#include "main.h"

int main(void){

  // Initialize pointSet struct for storing nav data
  pointSet points;
  points.currentStatusCode = 0;
  points.executionMode = 1;
  points.irErrInt = 0;
  points.lastErr = 0;

  // Initializ HWProfile struct for storing hardware info
  // and set up motors and GPIO
  HWProfile profile;
  initHardware(&profile);

  // Initialize timeTracker struct
  timeTracker time;
  time.iteration = 0;
  time.lastTime = 0;
  
  Printf("Hello World\n\n");

  // Initialize python listener
  initializeRasPy();
  
  while(true) {

    // Get ultrasonic sensor distances
    //getDistance(&points,&profile,&time);

    // Get line sensor reflectances
    getLineData(&points,&profile);

    setMotors(IRpidControl(&time,&points),1,&profile);
    // Computations
    //cartesian(&points);
    //indexObstacles(&points);

    // Update tracker
    updateTimeTracker(&time);
    
    // Print out info
    printOutInfo(&points,&profile,&time);

    ledColorError(&points);

    // Delay for 1 second
    LoopDelay(1);
  }
  
  return 0;
}
