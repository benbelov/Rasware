// Main.c in /Blackbird
// Goes ultrasonic
// Navigates around blocks


// All subroutines are in other files
#include "main.h"

// Global diagnostic info
int iteration = 0;

int main(void){

  // Initialize pointSet struct for storing nav data
  pointSet points;
  points.currentStatusCode = 0;

  // Initializ HWProfile struct for storing hardware info
  // and set up motors and GPIO
  HWProfile profile;
  initHardware(&profile);
  
  
  Printf("Hello World\n\n");
  
  while(true) {

  
    // Diagnostic info
    long loop_start_time = (long) GetTimeUS();
    

    // The actual function
    getDistance(&points,&profile);

    // Time computation
    long execution_time = (long) GetTimeUS() - loop_start_time;
    
    // Timestamp update
    iteration += 1;    
  
    // Print out info
    Printf("Iteration: %d\n",iteration);
    GhettoPrintf("Execution time (us)",execution_time);

    PrintOutDistances(&points);

    ledColorError(&points);
    
    LoopDelay(1);
    
  }
  return 0;
}
