// Main.c in /Blackbird
// Goes ultrasonic
// Navigates around blocks


// Subroutines in other files:
#include "main.h"


tMotor *left;
tMotor *right;

tBoolean led_on;
 
// How long should we wait for the distance sensors?
long timeout_us = 15000;

// Array containing the 5 trigger pair pin mappings
tPin trigger_pins[5] = {PIN_D0,PIN_D1,PIN_D2,PIN_D3,PIN_E1};

// Two echo pins
tPin echo_1 = PIN_E2;
tPin echo_2 = PIN_E3;

// Global diagnostic info
int iteration = 0;


// Initializes servos. LM4Fs have issues with motor directions and speeds. Fortunately, TM4Cs work fine.
// THIS WILL NOT WORK ON A LM4F
void initServos(void){
  left = InitializeServoMotor(PIN_E4, false);
  right = InitializeServoMotor(PIN_E5, true);
}


int main(void){

  pointSet points;
  
  Printf("Hello World\n\n");
  
  InitializeGPIO();
  initServos();
  
  while(true) {

    // Pointset
    Printf("test2\n");

  
    // Diagnostic info
    long loop_start_time = (long) GetTimeUS();
    
    // Heartbeat LED
    SetPin(PIN_F1, led_on);
    SetPin(PIN_F3, !led_on);
    led_on = !led_on;

    // The actual function
    getDistance(&points,timeout_us,trigger_pins,echo_1,echo_2);

    // Time computation
    long execution_time = (long) GetTimeUS() - loop_start_time;
    
    // Timestamp update
    iteration += 1;    
  
    // Print out info
    Printf("Iteration: %d\n",iteration);
    GhettoPrintf("Execution time (us)",execution_time);

    PrintOutDistances(&points);
    
    LoopDelay(1);
    
  }
  return 0;
}
