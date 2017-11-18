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


// If the button is pressed, cycle through execution modes.
void checkModeChange(HWProfile * profile, pointSet * points){

  if (profile->previousButtonState == 0 && GetPin(PIN_F4)) {
    points->executionMode += 1;
    points->executionMode %= 2;
    profile->previousButtonState = 1;
  }

  else {
    profile->previousButtonState = 0;
  }
}

// Wait for the start:
void waitForStart(pointSet * points, HWProfile * profile, timeTracker * time) {
  long CCW_counter = 0;
  long CW_counter = 0;
  int go_counter = 0;
  
  while(1) {
    
    getDistance(points,profile,time);
    if(points->r[9] > points->r[0]) {
      CCW_counter +=1;
    }
    else {
      CW_counter += 1;
    }

    if(points->r[5] < 10 || points->r[4] < 10) {
      go_counter += 1;
    }
    else if(go_counter > 0) {
      go_counter -= 1;
    }
    
    if(go_counter > 5) {
      if(CCW_counter > CW_counter) {
	points->direction = 1;
      }
      else {
	points->direction = 0;
      }
      break;
    }
  }
}
