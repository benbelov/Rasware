#include <stdlib.h>
#include <math.h>

// Raslib includes:
#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/uart.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/servo.h>
#include <RASLib/inc/linesensor.h>
#include <RASLib/inc/adc.h>

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
  // IR array raw output
  float reflectances[8];
  // IR array processed output
  char line[8];
  // output of indexObstacles
  int obstacleIndex[10];
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
   
// display.c
void GhettoPrintf(char *message,long number);
void PrintOutDistances(pointSet * points);
void PrintOutLine(pointSet * points);
void ledColorError(pointSet * points);
void updateTimeTracker(timeTracker * tracker);
void printTimeTracker(timeTracker * tracker);
void printOutInfo(pointSet * points, HWProfile * profile, timeTracker * time);

// raspy.c
void printToPySerial(pointSet * points,HWProfile * profile,timeTracker * time);
void initializeRasPy();
void definecolor(char * color, int r, int g, int b);
void setscale(float scale);
void drawline(float x_1,float y_1,float x_2,float y_2,char * color);
void setoffset(int x, int y);
void text(float x, float y, char * label, char * color);
void numtext(float x, float y, int label, char * color);
void floattext(float x, float y, float label, char * color);
void echo(char * message);
void echofloat(float message);
void echolong(long message);
void clrscrn();
void startraspy();
void stopraspy();
void drawbuffer();

// sensors.c
void LoopDelay(float time);
void PulsePin(tPin pin);
float * calculateDistance(long start_time_1, long end_time_1, long start_time_2, long end_time_2);
long getDistance(pointSet * points, HWProfile * profile, timeTracker *tracker);
int checkRamp(); // not done
int findFirstObstacle(pointSet * points,int n);
void getLineData(pointSet * points,HWProfile * profile);

// math.c
void cartesian(pointSet * points);
int isObtuse(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y);
float fastInverseTangent(float x);
int isConvex(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y);

// nav.c
void indexObstacles(pointSet * points);
float chooseTarget(pointSet * points); // not done
float pidControl(float target, float * targetHistory); // not done
float IRpidControl(timeTracker * tracker, pointSet * points);

// control.c
void setMotors(float pidCoefficient,float sensitivity,HWProfile * profile); // not done

// hardware.c
void initHardware(HWProfile * profile);
