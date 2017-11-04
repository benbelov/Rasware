// Raslib includes:
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/uart.h>
#include <RASLib/inc/motor.h>


// definition of the pointSet structure
// stores sets of points and distances
typedef struct pointSet pointSet;
struct pointSet {
  float x[10];
  float y[10];
  float r[10];
  int obstacleIndex[10];
};
   
// display.c
void GhettoPrintf(char *message,long number);
void PrintOutDistances(float *distances);

// sensors.c
void LoopDelay(float time);
void PulsePin(tPin pin);
float * calculateDistance(long start_time_1, long end_time_1, long start_time_2, long end_time_2);
float * getDistance(long timeout_us,tPin * trigger_pins,tPin echo_1, tPin echo_2);
int checkRamp(); // not done
int findFirstObstacle(pointSet * points,int n);

// math.c
void cartesian(pointSet * points); // not done
int isObtuse(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y);
float fastInverseTangent(float x);
int isConvex(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y);

// nav.c
void indexObstacles(pointSet * points);
float chooseTarget(pointSet * points); // not done
float pidControl(float target, float * targetHistory); // not done

// control.c
void setMotors(float pidCoefficient,float sensitivity); // not done
