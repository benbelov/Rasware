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

  // Initialize HWProfile struct for storing hardware info
  // and set up motors and GPIO
  HWProfile profile;
  initHardware(&profile);

  // Initialize timeTracker struct
  timeTracker time;
  time.iteration = 0;
  time.lastTime = 0;
  time.FPS_pointer = 0;

  // Init the pidProfile struct
  pidProfile pid;
  initpidProfile(&pid);
  
  Printf("Hello World\n\n");


  // Initialize python listener
  initializeRasPy();
  
  while(true) {

    // User input
    //checkModeChange(&profile,&points);
    
    // Get ultrasonic sensor distances
    getDistance(&points,&profile,&time);

    // Get line sensor reflectances
    getLineData(&points,&profile);

    // Computations
    cartesian(&points);
    indexObstacles(&points);
    chooseTarget(&points,&pid);
    float correction = pidControl(&pid);

    setMotors(correction,&profile);
    
    // Update tracker
    updateTimeTracker(&time);
    
    // Print out info
    printOutInfo(&points,&profile,&time,&pid);

    ledColorError(&points);

    // Delay for 1 second
    //LoopDelay(0.5);
  }
  
  return 0;
}
