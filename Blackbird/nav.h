#ifndef _NAV
#define _NAV

#include <stdlib.h>
#include <math.h>

#include "structures.h"
#include "math.h"
#include "raspy.h"

// nav.h in /Blackbird
// header file for navigation subroutines

// finds the first element of the input obstacle index
int findFirstObstacle(pointSet * points, int obstacle);

// indexes obstacles; modifies the pointSet directly
// assigns an int to each distance sensor measurement
// -1 = not in 'top' obstacle
// 0 = wall
// 1,2,... = obstacles, numbered
void indexObstacles(pointSet * points);

// Applies filters to distance sensor raw output
void filterDistances(pointSet * points);

// Finds valid target vectors
void findValidTargets(pointSet * points);

// Not yet implemented
float chooseTarget(pointSet * points);

// Not yet implemented
void initpidProfile(pidProfile * pidprofile);

// Not yet impelmented
float pidControl(float target, pidProfile * pidprofile); // not done

// Takes IR array output and applies PID control
// I and D terms currently disabled
float IRpidControl(timeTracker * tracker, pointSet * points);

#endif
