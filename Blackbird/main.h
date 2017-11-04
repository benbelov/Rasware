// definition of the pointSet structure
// stores sets of points and distances
typedef struct pointSet {
  float * x;
  float * y;
  float * r;
  int * obstacleIndex;
} pointSet;
   
// display.c
void GhettoPrintf(char *message,long number);
void PrintOutDistances(float *distances);

// sensors.c
void LoopDelay(float time);
void PulsePin(tPin pin);
float * calculateDistance(long start_time_1, long end_time_1, long start_time_2, long end_time_2);
float * getDistance(long timeout_us,tPin * trigger_pins,tPin echo_1, tPin echo_2);
int checkRamp(); // not done

// math.c
float * cartesian(pointSet points); // not done
int isObtuse(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y); // not done
float inverseTangent(float ratio); // not done

// nav.c
int * indexObstacles(pointSet points); // not done
float chooseTarget(pointSet points); // not done
float pidControl(float target, float * targetHistory); // not done

// control.c
void setMotors(float pidCoefficient,float sensitivity); // not done
