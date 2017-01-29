/*
Defines the 6 planes to capture the plane values for the frustum - used for frustum culling in camera class
*/

#ifndef __PLANE_H__
#define __PLANE_H__

#include "DirectXVector.h"

struct Plane{
	float a;
	float b;
	float c;
	float d;
};

#endif