#ifndef _RASPY
#define _RASPY

#include <RASLib/inc/common.h>
#include <RASLib/inc/uart.h>
#include <RASLib/inc/time.h>

#include "structures.h"

// raspy.h in /Blackbird
// Contains raspy library subroutines

// Prints out information in a machine readable format
// Intended to pipe info to PySerial
// Format: struct:var,var... \n struct:var,var; ... \n ...
void printToPySerial(pointSet * points,HWProfile * profile,timeTracker * time);

// Set initial values for the RasPy script to use
void initializeRasPy();

// Translation routines
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

#endif
