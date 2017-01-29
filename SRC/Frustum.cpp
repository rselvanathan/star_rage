/* 
This class contains the required methods to do frustum culling

Author : Romesh Selvanathan
Date : 03/05/2012

References:

Benstead,L, Astle, D, Hawkins, K. (2009).  Beginning OpenGL Game Programming, Second Edition. Boston: Course Technology.
*/
#include "Frustum.h"
#include "GameUtil.h"

/*
Update the Frustum, by obtaining the 6 sides

References:

Benstead,L, Astle, D, Hawkins, K. (2009).  Beginning OpenGL Game Programming, Second Edition. Boston: Course Technology.
*/
void Frustum::UpdateFrustum()
{
	GLfloat projection[16];		// Temp hold for projection matrix
	GLfloat modelview[16];		// Temp hold for modelview matrix
	GLfloat mvp[16];			// Hold the modelview-projecton matrix
	float length;				// used to normalize the plane values

	// Get current Projection and Modelview matrix
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	//Store current modelview matrix
	glPushMatrix();
		//Load the Strored projection Matrix()
		glLoadMatrixf(projection);
		// Multiply stored modelview matrix by projection matrix
		glMultMatrixf(modelview);
		// Read the result
		glGetFloatv(GL_MODELVIEW_MATRIX, mvp);
	//Restore former matrix
	glPopMatrix();

	// Now get the values that make up a plane and the plane formula is ax + by + cz + d = 0
	// abc are the three component of plane's normal and d is distance from the origin x, y, z

	// Near plane
	frustum[0].a = mvp[3] + mvp[2]; 
	frustum[0].b = mvp[7] + mvp[6]; 
	frustum[0].c = mvp[11] + mvp[10];
	frustum[0].d = mvp[15] + mvp[14];
	//Normalize the values
	length = sqrt((frustum[0].a*frustum[0].a) +(frustum[0].b*frustum[0].b )+(frustum[0].c*frustum[0].c )+(frustum[0].d*frustum[0].d));
	frustum[0].a /= length;
	frustum[0].b /= length;
	frustum[0].c /= length;
	frustum[0].d /= length;

	//Far Plane
	frustum[1].a = mvp[3] - mvp[2]; 
	frustum[1].b = mvp[7] - mvp[6]; 
	frustum[1].c = mvp[11] - mvp[10];
	frustum[1].d = mvp[15] - mvp[14];
	//Normalize the values
	length = sqrt((frustum[1].a*frustum[1].a) +(frustum[1].b*frustum[1].b )+(frustum[1].c*frustum[1].c )+(frustum[1].d*frustum[1].d));
	frustum[1].a /= length;
	frustum[1].b /= length;
	frustum[1].c /= length;
	frustum[1].d /= length;

	//Bottom Plane
	frustum[2].a = mvp[3] + mvp[1]; 
	frustum[2].b = mvp[7] + mvp[5]; 
	frustum[2].c = mvp[11] + mvp[9];
	frustum[2].d = mvp[15] + mvp[13];
	//Normalize the values
	length = sqrt((frustum[2].a*frustum[2].a) +(frustum[2].b*frustum[2].b )+(frustum[2].c*frustum[2].c )+(frustum[2].d*frustum[2].d));
	frustum[2].a /= length;
	frustum[2].b /= length;
	frustum[2].c /= length;
	frustum[2].d /= length;

	//Top Plane
	frustum[3].a = mvp[3] - mvp[1]; 
	frustum[3].b = mvp[7] - mvp[5]; 
	frustum[3].c = mvp[11] - mvp[9];
	frustum[3].d = mvp[15] - mvp[13];
	//Normalize the values
	length = sqrt((frustum[3].a*frustum[3].a) +(frustum[3].b*frustum[3].b )+(frustum[3].c*frustum[3].c )+(frustum[3].d*frustum[3].d));
	frustum[3].a /= length;
	frustum[3].b /= length;
	frustum[3].c /= length;
	frustum[3].d /= length;

	//Left Plane
	frustum[4].a = mvp[3] + mvp[0]; 
	frustum[4].b = mvp[7] + mvp[4]; 
	frustum[4].c = mvp[11] + mvp[8];
	frustum[4].d = mvp[15] + mvp[12];
	//Normalize the values
	length = sqrt((frustum[4].a*frustum[4].a) +(frustum[4].b*frustum[4].b )+(frustum[4].c*frustum[4].c )+(frustum[4].d*frustum[4].d));
	frustum[4].a /= length;
	frustum[4].b /= length;
	frustum[4].c /= length;
	frustum[4].d /= length;

	//Right Plane
	frustum[5].a = mvp[3] + mvp[0]; 
	frustum[5].b = mvp[7] + mvp[4]; 
	frustum[5].c = mvp[11] + mvp[8];
	frustum[5].d = mvp[15] + mvp[12];
	//Normalize the values
	length = sqrt((frustum[5].a*frustum[5].a) +(frustum[5].b*frustum[5].b )+(frustum[5].c*frustum[5].c )+(frustum[5].d*frustum[5].d));
	frustum[5].a /= length;
	frustum[5].b /= length;
	frustum[5].c /= length;
	frustum[5].d /= length;
}

// Check if a point is inside the frustum
bool Frustum::PointInFrustum(float x, float y, float z)
{
	for (int p = 0; p < 6; p++)
    {
		if (frustum[p].a * x + 
            frustum[p].b * y + 
            frustum[p].c * z + 
            frustum[p].d < 0)
        {
			return false;
        }
    }
	return true;
}

//Check if a spehere is inside the frustum
bool Frustum::SphereInFrustum(float x, float y, float z, float radius)
{
	for (int p = 0; p < 6; p++)
    {
		if (frustum[p].a * x + 
            frustum[p].b * y + 
            frustum[p].c * z + 
            frustum[p].d <= -radius)
        {
			return false;
        }
    }
	return true;
}