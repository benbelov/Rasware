// Nav.c in /Blackbird
// Contains navigation algorithm functions
#include "main.h"
#include <math.h>

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

      // if two blocks are next to each other,
      // and happen to form a convex angle relative to the robot
      else if (isConvex((points->x)[n-1],(points->y)[n-1],
			(points->x)[n],(points->y)[n],
			(points->x)[n+1],(points->y)[n+1]) == 1 ) {
	obstacleNumber += 1;
	(points->obstacleIndex)[n] = obstacleNumber;
      }

      // if two blocks are next each other,
      // form a concave angle,
      // but take up more than four inches worth of width

      // see if the distance between the obstacle start
      // and current is greater than 4
      else if(pow((points->x)[findFirstObstacle(points,n)] -
		  (points->x)[n],2) +
	      pow((points->y)[findFirstObstacle(points,n)] -
		  (points->y)[n],2)
	      > 4) {
	obstacleNumber += 1;
	(points->obstacleIndex)[n] = obstacleNumber;
      }

      else {
	(points->obstacleIndex)[n] = obstacleNumber;
      }      
    }
  }
}
