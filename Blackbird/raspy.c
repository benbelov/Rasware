// raspy.c in /Blackbird
// contains rubroutines for interacting with RasPy

#include "main.h"


// Translation routines
void definecolor(char * color, int r, int g, int b) {
  Printf("definecolor:%s,%d,%d,%d\n",color,r,g,b);
}
void setscale(float scale) {
  Printf("setscale:%f\n",scale);
}
void drawline(float x_1,float y_1,float x_2,float y_2,char * color) {
  Printf("drawline:%f,%f,%f,%f,%s\n",x_1,y_1,x_2,y_2,color);
}
void setoffset(int x, int y) {
  Printf("setoffset:%d,%d\n",x,y);
}
void text(float x, float y, int label) {
  Printf("text:%f,%f,%d\n",x,y,label);
}
void clrscrn() {
  Printf("clrscrn\n");
}
void startraspy() {
  Printf("start\n");
}

// Set initial values for the RasPy script to use
void initializeRasPy() {
  startraspy();
  clrscrn();
  setscale(5);
  definecolor("red",1,0,0);
  definecolor("green",0,1,0);
  definecolor("blue",0,0,1);
  definecolor("yellow",1,1,0);
  definecolor("purple",1,0,1);
  definecolor("orange",1,0.5,0);
}


// Prints out information in a machine readable format
// Intended to pipe info to PySerial
// Format: struct:var,var... \n struct:var,var; ... \n ...
void printToPySerial(pointSet * points,HWProfile * profile,timeTracker * time) {

  clrscrn();
  
  for (int i=0; i<10; i++) {
    if(points->obstacleIndex[i] == -1) {
      drawline(0,0,points->x[i],points->y[i],"red");
    }
    else if(points->obstacleIndex[i] == 0) {
       drawline(0,0,points->x[i],points->y[i],"black");
    }
    else if(points->obstacleIndex[i] == 1) {
       drawline(0,0,points->x[i],points->y[i],"green");
    }
    else if(points->obstacleIndex[i] == 2) {
       drawline(0,0,points->x[i],points->y[i],"blue");
    }
    else if(points->obstacleIndex[i] == 3) {
       drawline(0,0,points->x[i],points->y[i],"yellow");
    }
    else if(points->obstacleIndex[i] == 4) {
       drawline(0,0,points->x[i],points->y[i],"purple");
    }
    else if(points->obstacleIndex[i] == 5) {
       drawline(0,0,points->x[i],points->y[i],"orange");
    }
    
    text(points->x[i],points->y[i],points->obstacleIndex[i]);
  }
}
