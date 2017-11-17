// Nav.c in /Blackbird
// Contains navigation algorithm functions

#include "nav.h"

// finds the first element of the input obstacle index
int findFirstObstacle(pointSet * points, int obstacle) {
  // find the start index of the current obstacle
  int currentObstacle = 0;
  int i = 9;
  while(obstacle != (points->obstacleIndex)[i] && i>=0) {
    i -= 1;
  }
  return(i);
}


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

// Finds valid target vectors
void findValidTargets(pointSet * points) {

  // Reset the total vector counter
  points->vectorCount = 1;
    
  for (int i=9; i>0; i--) {
    
    float left_bound;
    float right_bound;
    float target_vector;
    float vector_distance;
    int isValid = 0;
    
    // If the point we're looking at is the end of the wall:
    if (points->obstacleIndex[i] == 0 &&
        points->obstacleIndex[i-1] != 0) {
      left_bound = i*PI/9;
      target_vector = left_bound - asinf(3/points->r[i]);
      vector_distance = 3/tanf(left_bound - target_vector);
      right_bound = target_vector - atanf(6/vector_distance);
    }

    
    // Else, if the point we're looking at is a block:
    else if (points->obstacleIndex[i] != 0 &&
    	     points->obstacleIndex[i] != -1) {
      left_bound = i*PI/9;
      right_bound = i*PI/9 - 2*asinf(6/points->r[i]);
      target_vector = (left_bound + right_bound)/2;
      vector_distance = pow(pow(points->r[i],2) - 36, 1/2);
    }

    int j = 9;
    while (j>0 && isValid == 1) {

      // If a block is in the cleared sector, and it is within 10":
      if (left_bound > 20*j && 20*j > right_bound
	  &&
	  pow(points->x[i] - points->x[j],2) +
	  pow(points->y[i] - points->y[j],2) < 64
	  &&
	  points->obstacleIndex[i] != -1) {

	  isValid = 0;
      }
      j -= 1;
    }
    
    // If the path has survived all the tests
    if (isValid == 1) {
      points->validVectors[(points->vectorCount)-1] = target_vector;
      points->validVectorLengths[(points->vectorCount)-1] = vector_distance;
      points->vectorCount += 1;
    }
    
  }
  
}

// Not yet implemented
float chooseTarget(pointSet * points) {

  
  
}

// Takes IR array output and applies PID control
// I and D terms currently disabled
float IRpidControl(timeTracker * tracker, pointSet * points)
{
  float kp = 1;
  //float ki = 0.5;
  //float kd = 1000;

  int line01 = 0;

  if(tracker->lastTime == 0)
  {
    tracker->lastTime = (long) GetTimeUS();
  }

  long nowTime = (long) GetTimeUS();
  long dTime = nowTime - tracker->lastTime;
  float error = 0;
  for(int sensor=0; sensor<5; sensor++)
  {
    if((points->line)[sensor] `== '1')
    {
      error += (sensor-2)/5.0;
      line01 = 1;
    }
  }

  points->irErrInt += (error * dTime);

  //float dErr = (error - points->lastErr) / dTime;

  float out = kp * error;// + kd * dErr; //+ ki * points->irErrInt
  
  if(line01 == 0)
  {
    if(points->lastErr < 0)
    {
      error = -1;
    }
    else if (points->lastErr == 0)
    {
      error = 0;
    }
    else
    {
      error = 1;
    }
  }

  points->lastErr = error;
  tracker->lastTime = nowTime;

  return out;
}
