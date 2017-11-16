#ifndef _STRUCTURES
#define _STRUCTURES

// structures.h in /Blackbird
// contains structure definitions

#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>

// definition of the pointSet structure
// stores sets of points and distances
typedef struct pointSet pointSet;
struct pointSet {
  // x,y coordinates, computed from r
  float x[10];
  float y[10];
  // distance sensor raw output
  float r[10];
  // IR array raw output
  float reflectances[8];
  // IR array processed output
  char line[8];
  // output of indexObstacles
  int obstacleIndex[10];
  // valid path vectors
  float validVectors[10];
  float validVectorLengths[10];
  int vectorCount;
  // 0 = display heartBeat; 1 = display errorCode; 2 = display executionMode
  int currentStatusCode;
  int heartBeat;
  int errorCode;
  int executionMode;
  // integral term for IR PID and previous error
  float irErrInt;
  float lastErr;
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
  tServo *small;
  tADC *linesensor[5];
  float threshhold;
  int previousButtonState;
};

// definition of the main diagnostic structure
// stores time-related information
typedef struct timeTracker timeTracker;
struct timeTracker {
  int iteration;
  long execution_time;
  long lastTime;
  float FPS[20];
  int FPS_pointer;
  float avgFPS;
};

#endif
