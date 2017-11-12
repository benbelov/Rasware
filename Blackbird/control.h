#ifndef _CONTROL
#define _CONTROL

#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>

#include "structures.h"

// control.h in /Blackbird
// Contains motor-control subroutines

// Sets motor speeds based on an input pidCoefficient and sensitivity
void setMotors(float pidCoefficient,float sensitivity,HWProfile * profile);

#endif
