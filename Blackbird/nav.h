// nav.h in /Blackbird
// header file for navigation subroutines

#ifndef _NAV
#define _NAV

#include <stdlib.h>
#include <math.h>

#include "structures.h"
#include "math.h"
#include "raspy.h"

// indexes obstacles; modifies the pointSet directly
// assigns an int to each distance sensor measurement
// -1 = not in 'top' obstacle
// 0 = wall
// 1,2,... = obstacles, numbered
void indexObstacles(pointSet * points);

// Not yet implemented
void chooseTarget(pointSet * points, pidProfile * pid);

// Not yet implemented
void initpidProfile(pidProfile * pidprofile);

// Not yet impelmented
float pidControl(pidProfile * pidprofile);

#endif
