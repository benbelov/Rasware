#ifndef _HARDWARE
#define _HARDWARE

#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/adc.h>

#include "structures.h"

// hardware.h in /Blackbird
// contains hardware settings and initialization subroutines

// initializes hardware profile and sets values
void initHardware(HWProfile * profile);

// checks if the button is pressed
void checkModeChange(HWProfile * profile, pointSet * points);

#endif
