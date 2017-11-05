// Main.c in /Blackbird
// Goes ultrasonic
// Navigates around blocks

// All subroutines are in other files
#include "main.h"

int main(void){

  // Initialize pointSet struct for storing nav data
  pointSet points;
  points.currentStatusCode = 0;

  // Initializ HWProfile struct for storing hardware info
  // and set up motors and GPIO
  HWProfile profile;
  initHardware(&profile);

  // Initialize timeTracker struct
  timeTracker time;
  time.iteration = 0;

  // Main function diagnostic info
  int iteration = 0;
  
  Printf("Hello World\n\n");
  
  while(true) {

    // The actual function
    getDistance(&points,&profile,&time);
    
    // Update tracker
    updateTimeTracker(&time);
    
    // Print out info
    PrintOutDistances(&points);
    ledColorError(&points);

    // Delay for 1 second
    LoopDelay(1);
  }
  
  return 0;
}
