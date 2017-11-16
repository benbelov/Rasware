// hardware.c in /Blackboard
// Contains hardware related functions

#include "hardware.h"

// initHardware
// initializes hardware profile and sets values
void initHardware(HWProfile * profile) {

  InitializeUART(115200);
  
  // Initialize GPIO
  InitializeGPIO();

  // Distance sensor constants
  tPin trigger_pins_temp[5] = {PIN_D0,PIN_D1,PIN_D2,PIN_D3,PIN_E1};
  for(int i=0;i<5;i++) {
    (profile->trigger_pins)[i] = trigger_pins_temp[i];
  profile->echo_1 = PIN_E2;
  }
  profile->echo_2 = PIN_E3;
  profile->timeout_us = 15000;

  // IR sensor constants
  (profile->linesensor)[0] = InitializeADC(PIN_B5);
  (profile->linesensor)[1] = InitializeADC(PIN_E4);
  (profile->linesensor)[2] = InitializeADC(PIN_E5);
  (profile->linesensor)[3] = InitializeADC(PIN_B4);
  (profile->linesensor)[4] = InitializeADC(PIN_E0);
  profile->threshhold = 0.5;
  
  // Initialize servo motors
  profile->left = InitializeServoMotor(PIN_A4,false);
  profile->right = InitializeServoMotor(PIN_B7,true);
  profile->small = InitializeServo(PIN_B6);

  // Start the button state at 0.
  profile->previousButtonState = 0;

}


// Toggles between UART and PySerial
void toggle(void){

  // Each time the button is pressed, increment executionMode by 1.

  if (profile->previousButtonState == 0 && GetPin(PIN_F4){

    points->executionMode += 1;
    points->executionMode %= 2;
    profile->previousButtonState = 1;

  }

  else {

    profile->previousButtonState = 0;
 
  }
}
