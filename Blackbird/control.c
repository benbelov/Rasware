
#include "main.h"

void setMotors(float pidCoefficient,float sensitivity, HWProfile * profile)
{
	float left;
	float right;

	left = 0.5 - pidCoefficient/2;
	right = 0.5 + pidCoefficient/2;

	SetMotors(profile->left, left);
	SetMotors(profile->right, right);
}