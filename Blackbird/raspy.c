// raspy.c in /Blackbird
// contains rubroutines for interacting with RasPy

#include "raspy.h"


// Translation routines
void definecolor(char * color, int r, int g, int b) {
  Printf("definecolor:%s,%d,%d,%d\n",color,r,g,b);
}
void setscale(float scale) {
  Printf("setscale:%f\n",scale);
}
void setoffset(int x, int y) {
  Printf("setoffset:%d,%d\n",x,y);
}
void drawline(float x_1,float y_1,float x_2,float y_2,char * color) {
  Printf("drawline:%f,%f,%f,%f,%s\n",x_1,y_1,x_2,y_2,color);
}
void drawray(float x_1,float y_1,float r,float theta,char * color) {
  Printf("drawray:%f,%f,%f,%f,%s\n",x_1,y_1,r,theta,color);
}
void drawcircle(float x, float y, float r, char * color) {
  Printf("drawcircle:%f,%f,%f,%s\n",x,y,r,color);
}
void text(float x, float y, char * label, char * color) {
  Printf("text:%f,%f,%s,%s\n",x,y,label,color);
}
void numtext(float x, float y, int label, char * color) {
  Printf("text:%f,%f,%d,%s\n",x,y,label,color);
}
void floattext(float x, float y, float label, char * color) {
  Printf("text:%f,%f,%f,%s\n",x,y,label,color);
}
void echo(char * message) {
  Printf("echo:%s\n",message);
}
void echofloat(float message) {
  Printf("echo:%f\n",message);
}
void echolong(long message) {
  Printf("echol:%d,%d\n",message/10000, message%10000);
}
void clrscrn() {
  Printf("clrscrn\n");
}
void stopraspy() {
  Printf("stop\n");
}
void startraspy() {
  Printf("start\n");
}
void drawbuffer() {
  Printf("draw\n");
}

// Set initial values for the RasPy script to use
void initializeRasPy() {
  startraspy();
  clrscrn();
  setoffset(400,100);
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
  
  for (int i=1; i<9; i++) {
    if(points->obstacleIndex[i] == -1) {
      drawline(0,0,points->x[i],points->y[i],"red");
    }
    else if(points->obstacleIndex[i] == points->obstacleIndex[i+1]) {
      if(points->obstacleIndex[i] == 0) {
	drawline(points->x[i],points->y[i],
		 points->x[i+1],points->y[i+1],"black");
      }
      else if(points->obstacleIndex[i] == 1) {
	drawline(points->x[i],points->y[i],
		 points->x[i+1],points->y[i+1],"blue");
      }
      else if(points->obstacleIndex[i] == 2) {
	drawline(points->x[i],points->y[i],
		 points->x[i+1],points->y[i+1],"purple");
      }
      else if(points->obstacleIndex[i] == 3) {
	drawline(points->x[i],points->y[i],
		 points->x[i+1],points->y[i+1],"orange");
      }
      else if(points->obstacleIndex[i] == 4) {
	drawline(points->x[i],points->y[i],
		 points->x[i+1],points->y[i+1],"green");
      }
    }

    if(points->obstacleIndex[i] == 1) {
      drawcircle(points->x[i],points->y[i],3,"blue");
    }
    else if(points->obstacleIndex[i] == 2) {
      drawcircle(points->x[i],points->y[i],3,"purple");
    }
    else if(points->obstacleIndex[i] == 3) {
      drawcircle(points->x[i],points->y[i],3,"orange");
    }
    else if(points->obstacleIndex[i] == 4) {
      drawcircle(points->x[i],points->y[i],3,"green");
    }
    else if(points->obstacleIndex[i] != 0) {
      drawcircle(points->x[i],points->y[i],3,"black");
    }

    numtext(points->x[i],points->y[i],points->obstacleIndex[i],"black");
  }

  floattext(-35,-7.5,time->avgFPS,"black");

  for(int i=0; i<10; i++) {
    floattext(-60,10*i,points->r[i],"black");
  }
  
  // Draw possible target vectors
  for(int i=0; i<(points->vectorCount); i++) {
    drawray(0,0,points->validVectorLengths[i],points->validVectors[i],"blue");
  }
  // draw the buffer
  drawbuffer();
  
}
