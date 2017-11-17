// control.c in /Blackbird
// Contains motor control subroutines

#include "control.h"

// Sets motor speeds based on an input pidCoefficient and sensitivity
void setMotors(float pidCoefficient,HWProfile * profile) {

  left = linearizeMotors(pidCoefficient)[0];
  right = linearizeMotors(pidCoefficient)[1];

  SetMotor(profile->left, left);
  SetMotor(profile->right, right);
}

// Linearize motor profile
float * linearizeMotors(float pidCoefficient) {
}
