#ifndef _MATH
#define _MATH

// main.h in /Blackbird
// Header file for math related subroutines

#include <stdlib.h>
#include <math.h>

#include "structures.h"

// checks if an angle <ABC between three cartesian coordinates is obtuse
// returns 1 if obtuse, 0 if not obtuse
int isObtuse(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y);

// uses basic geometry to determine whether (b_x,b_y) is convex relative x=0
int isConvex(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y);

// fast inverse tangent; works very well for (-1,1);
// if the heading is mostly forward, use dx/dy; if the heading is far from forward, use dy/dx
// from http://nghiaho.com/?p=997
float fastInverseTangent(float x);

// compute x and y from r in a pointSet struct
// uses sin and cosine tables
// modifies the pointSet struct directly
void cartesian(pointSet * points);

#endif
