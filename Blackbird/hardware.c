// hardware.c in /Blackboard
// Contains hardware related functions

#include "main.h"

// initHardware
// initializes hardware profile and sets values
void initHardware(HWProfile * profile) {

  // Distance sensor constants
  tPin trigger_pins_temp[5] = {PIN_D0,PIN_D1,PIN_D2,PIN_D3,PIN_E1};
  for(int i=0;i<5;i++) {
    (profile->trigger_pins)[i] = trigger_pins_temp[i];
  }
  profile->echo_1 = PIN_E2;
  profile->echo_2 = PIN_E3;
  profile->timeout_us = 15000;

  // Initialize servo motors
  profile->left = InitializeServoMotor(PIN_E4,false);
  profile->right = InitializeServoMotor(PIN_E5,true);

  // Initialize GPIO
  InitializeGPIO();
}
