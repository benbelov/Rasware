// display.c
void GhettoPrintf(char *message,long number);
void PrintOutDistances(float *distances);

// sensors.c
void LoopDelay(float time);
void PulsePin(tPin pin);
float * calculateDistance(long start_time_1, long end_time_1, long start_time_2, long end_time_2);
float * getDistance(long timeout_us,tPin * trigger_pins,tPin echo_1, tPin echo_2);
