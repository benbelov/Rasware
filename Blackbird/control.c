// control.c in /Blackbird
// Contains motor control subroutines

#include "main.h"

void setMotors(float pidCoefficient,float sensitivity, HWProfile * profile)
{
	float left;
	float right;

	left = 0.2 - pidCoefficient/4;
	right = 0.2 + pidCoefficient/4;

	SetMotor(profile->left, left);
	SetMotor(profile->right, right);
}
