// control.c in /Blackbird
// Contains motor control subroutines

#include "control.h"

// Sets motor speeds based on an input pidCoefficient and sensitivity
void setMotors(float pidCoefficient,HWProfile * profile) {
	float defaultPower = 0.5;
	float pidScale = 1.0;

  float left = linearizeMotors(defaultPower - pidScale*pidCoefficient);
  float right = linearizeMotors(defaultPower + pidScale*pidCoefficient);

  SetMotor(profile->left, left);
  SetMotor(profile->right, right);
}

// Linearize motor profile
/*
*
*  y -function output   x- pidCoefficient arguement
*	0.05			0.109375
*	0.10			0.359375
*	0.15			0.53125
*	0.20			0.71875
*	0.30			0.9375
*	0.40			1
*
*/
/* (yhigh - ylow)/(xhigh - xlow) = slope between two adjacent points. 
 * y = ylow + slope(pidCoefficient - xlow), where ylow is the y value below the pidCoefficient arguement, 
 * and yhigh is the y value above it.
 */
float linearizeMotors(float pidCoefficient) {

	float yValues[] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4};
	float xValues[] = {0, 0.109375, 0.359375, 0.53125, 0.71875, 0.9375, 1};

	int neg = 1;

	if (pidCoefficient > 1){

		return 1;

	}
	if (pidCoefficient < 0 && pidCoefficient >= -1){
		neg = -1;
		pidCoefficient = -pidCoefficient;
	}

	for(int index = 1; index < 7; index++){

		if (pidCoefficient <= xValues[index]){
			return neg * (yValues[index-1] + 
			(((yValues[index] - yValues[index-1])/(xValues[index] - xValues[index-1]))*
			(pidCoefficient - xValues[index-1])));
		}
	}		
	return -1;

}
