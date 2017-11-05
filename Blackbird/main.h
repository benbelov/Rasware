#include <stdlib.h>

// Raslib includes:
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/uart.h>
#include <RASLib/inc/motor.h>

// Stellarisware include:
#include <StellarisWare/driverlib/rom.h>


// definition of the pointSet structure
// stores sets of points and distances
typedef struct pointSet pointSet;
struct pointSet {
  // x,y coordinates, computed from r
  float x[10];
  float y[10];
  // distance sensor raw output
  float r[10];
  // output of indexObstacles
  int obstacleIndex[10];
  // 0 = display heartBeat; 1 = display errorCode; 2 = display executionMode
  int currentStatusCode;
  int heartBeat;
  int errorCode;
  int executionMode;
};

// definition of the hardware profile structure
// stores all information about hardware settings
typedef struct HWProfile HWProfile;
struct HWProfile {
  tPin trigger_pins[5];
  tPin echo_1;
  tPin echo_2;
  long timeout_us;
  tMotor *left;
  tMotor *right;
};
   
// display.c
void GhettoPrintf(char *message,long number);
void PrintOutDistances(pointSet * points);
void ledColorError(pointSet * points);

// sensors.c
void LoopDelay(float time);
void PulsePin(tPin pin);
float * calculateDistance(long start_time_1, long end_time_1, long start_time_2, long end_time_2);
void getDistance(pointSet * points, HWProfile * profile);
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

// hardware.c
void initHardware(HWProfile * profile);
