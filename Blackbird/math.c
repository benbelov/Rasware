// math.c in /Blackbird
// Math routines for the navigation algorithm

#include "math.h"

// trig tables
const float sinTable[] = {0,             0.34202014332,
			  0.64278760968, 0.86602540378,
			  0.98480775301, 0.98480775301,
			  0.86602540378, 0.64278760968,
			  0.34202014332, 0             };
  
const float cosTable[] = {1,             0.93969262078,
			  0.76604444311, 0.5,
			  0.17364817766, -0.17364817766,
			  -0.5,          -0.76604444311,
			  -0.93969262078, -1            };


// checks if an angle <ABC between three cartesian coordinates is obtuse
// returns 1 if obtuse, 0 if not obtuse
int isObtuse(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y) {

  // using the pythagorean theorem:
  float index_1 = pow(a_x-b_x,2) + pow(a_y-b_y,2)
                + pow(b_x-c_x,2) + pow(b_y-c_y,2);
  float index_2 = pow(c_x-a_x,2) + pow(c_y-a_y,2);

  if(index_1 > index_2) {
    return(0);
  }
  else {
    return(1);
  }
}

// uses basic geometry to determine whether (b_x,b_y) is convex relative x=0
int isConvex(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y) {

  float bLineValue = (b_x - a_x) * (c_y - a_y) / (c_x - a_x) + a_y;

  if (bLineValue < b_y) {
    return(1);

  }
  else {
    return(0);
  }

  
}

// fast inverse tangent; works very well for (-1,1);
// if the heading is mostly forward, use dx/dy; if the heading is far from forward, use dy/dx
// from http://nghiaho.com/?p=997
float fastInverseTangent(float x) {
  return 57.2957*(0.785398*x - x*(fabs(x) - 1)*(0.2447 + 0.0663*fabs(x)));
}


// compute x and y from r in a pointSet struct
// uses sin and cosine tables
// modifies the pointSet struct directly
void cartesian(pointSet * points) {
  for (int n=0; n<10; n++) {
    (points->x)[n] = cosTable[n] * (points->r)[n];
    (points->y)[n] = sinTable[n] * (points->r)[n];
  }
}
