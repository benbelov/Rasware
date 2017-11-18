// Nav.c in /Blackbird
// Contains navigation algorithm functions

#include "nav.h"

// indexes obstacles; modifies the pointSet directly
// assigns an int to each distance sensor measurement
// -1 = not in 'top' obstacle
// 0 = wall
// 1,2,... = obstacles, numbered
void indexObstacles (pointSet * points) {

  int obstacleNumber = 0;

  points->obstacleIndex[9] = 0;
  
  for (int n=8; n>0; n--) {

    // if we're still reading the wall:
    if (obstacleNumber == 0) {

      // n_r = 17000 (timeout)
      if ((points->r)[n] >= 100) {
	obstacleNumber += 1;
	(points->obstacleIndex)[n] = -1;
      }

      // the wall can't form an acute angle relative to the robot
      // if an acute angle is formed, the right point must be an obstacle
      else if (isObtuse((points->x)[n-1],(points->y)[n-1],
			(points->x)[n],(points->y)[n],
			(points->x)[n+1],(points->y)[n+1]) == 0) {
	obstacleNumber += 1;
	(points->obstacleIndex)[n] = 0;
      }

      else if (n > 2) {
	obstacleNumber += 1;
	points->obstacleIndex[n] = 1;
      }

      // otherwise, the wall must just be continuing.
      else {
        (points->obstacleIndex)[n] = 0;
      }
    }
    
    // if we're no longer reading the wall
    else {
      // timeout obviously indicates a gap between obstacles
      if ((points->r)[n] >= 100) {
	obstacleNumber += 1;
	(points->obstacleIndex)[n] = -1;
      }

      // if two blocks are next each other,
      // but are further than 6" apart

      // see if the distance between the obstacle start
      // and current is greater than 4
      else if(pow((points->x)[n-1] -
		  (points->x)[n],2) +
	      pow((points->y)[n-1] -
		  (points->y)[n],2)
	      > 25) {
	(points->obstacleIndex)[n] = obstacleNumber;
	obstacleNumber += 1;
      }

      else {
	(points->obstacleIndex)[n] = obstacleNumber;
      }      
    }
  }
}


// Choose the target
void chooseTarget(pointSet * points, pidProfile * pid) {

  // The base vector is a vector parallel to the wall,
  // as measured by sensors 9 and 8.
  float base_vector = atan((points->y[9] - points->y[8])/
			  (points->x[9] - points->x[8]));
  if(base_vector < 0) {
    base_vector += PI;
  }

  // Desired distance from the wall for wall follow
  // Keep above 5cm, since sensors are unreliable at less than 5cm.
  float desired_distance = 10;

  // Check for obstacles in the path within 50cm
  float left_bound_base = points->x[9];
  float right_bound_base = left_bound_base + 20/cosf(base_vector - PI/2);

  int path_blocked = 0;
  for (int i=0; i<10; i++) {
    float left_bound = left_bound_base + points->y[i] * tanf(base_vector);
    float right_bound = right_bound_base + points->y[i] * tanf(base_vector);
    if (left_bound < points->x[i] &&
	points->x[i] < right_bound &&
	points->r[i] < 75) {
      path_blocked = 1;
      break;
    }
  }

  // No obstacles in the path within 50cm
  // Wall follow mode
  if (path_blocked = 0) {
    pid->target_vector = base_vector;
    pid->wall_follow_correction = points->r[8] - desired_distance;
  }

  // Obstacles in the path within 50cm
  // Go to the outside of them
  else {

    // The error state is 0.
    pid->target_vector = 0;
    
    for (int i=9; i>0; i--) {

      // The path starts out being invalid.
      int isValid = 0;
      float left_bound;
      float right_bound;
      float target_vector;
      
      // For the end of each obstacle, start off assuming that it's valid
      if (points->obstacleIndex[i] > 0 &&
	  points->obstacleIndex[i] != points->obstacleIndex[i+1]) {
        left_bound = i*PI/9;
	if (points->r[i] < 16) {
	  right_bound = i*PI/9 - PI/2;
	}
	else {
	  right_bound = i*PI/9 - 2*asinf(16/points->r[i]);
	}
	target_vector = (left_bound + right_bound)/2;
	isValid = 1;
      }

      // Check for obstacles in the way
      int j = i-1;
      while (isValid == 1 && j>0) {
	// If there's an obstacle within the sector and within 50cm
	if (left_bound > j*PI/9 &&
	    j*PI/9 > right_bound &&
	    points->r[j] < points->r[i] + 50) {
	  isValid = 0;
	}
	j--;
      }

      // If we pass all the tests
      if (isValid == 1) {
	pid->target_vector = target_vector;
	pid->wall_follow_correction = 0;
	break;
      }
    }
  }
  
  // If we still can't find a target vector
  if (pid->target_vector < PI/9) {
    pid->target_vector = base_vector;
    pid->wall_follow_correction = points->r[9] - desired_distance;
  }

}


// Initialize values for pid control
void initpidProfile(pidProfile * pidprofile) {

  // PID coefficients
  pidprofile->k_p = 0.4;
  pidprofile->k_d = 0.2;
  pidprofile->k_i = 0.2;

  // Initialize the time tracker for pid
  pidprofile->previousTime = GetTimeUS();
  pidprofile->previousCorrection = 0;
  pidprofile->dpreviousCorrection = 0;
}

// Calculate PID control
float pidControl(pidProfile * pid) {
  
  // The base is the previous correction
  // Add on terms for the proportion, previous change,
  // and wall follow correction (integral)
  float output = (pid->k_p * (pid->target_vector - PI/2) +
		  pid->k_d * pid->dpreviousCorrection +
		  pid->k_i * pid->wall_follow_correction);

  echofloat(output);

  // Compute the time change
  float dTime = GetTimeUS() - pid->previousTime;
  // Update the previous time
  pid->previousTime = GetTimeUS();
  // Update the derivative
  if (dTime == 0) {
    pid->dpreviousCorrection = 0;
  }
  else {
   pid->dpreviousCorrection = (output - pid->previousCorrection)/(dTime);
  }
  // Update the previous correction
  pid->previousCorrection = output;

  return(output);
  
}
