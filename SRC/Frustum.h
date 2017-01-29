/* 
This class contains the required methods to do frustum culling

Author : Romesh Selvanathan
Date : 03/05/2012

References:

Benstead,L, Astle, D, Hawkins, K. (2009).  Beginning OpenGL Game Programming, Second Edition. Boston: Course Technology.
*/
#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include "Plane.h"

class Frustum
{
public:
	// Update the view frustum
	void UpdateFrustum();
	// Check if a point is inside the frustum
	bool PointInFrustum(float x, float y, float z);
	//Check if a spehere is inside the frustum
	bool SphereInFrustum(float x, float y, float z, float radius);

private:
	Plane frustum[6];								// the viewing frustum
};

#endif