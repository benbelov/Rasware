// math.c in /Blackbird
// Math routines for the navigation algorithm

#include <math.h>

// checks if an angle <ABC between three cartesian coordinates is obtuse
// returns 1 if obtuse, 0 if not obtuse
int isObtuse(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y) {

  // using the pythagorean theorem:
  float index_1 = pow(a_x-b_x,2) + pow(a_y-b_y,2)
                + pow(b_x-c_x,2) + pow(b_y-c_y,2);
  float index_2 = pow(c_x-c_a,2) + pow(c_y-a_y,2);

  if(index_1 > index_2) {
    return(0);
  }
  else {
    return(1);
  }
}
