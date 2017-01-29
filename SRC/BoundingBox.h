/*
A child class of the BoundingShape class which represent a Bounding Box. It is a Axis Aligned Bounding box not object Aligned.
A bounding box is a very fast type of collision check just like the sphere however it does not fit well for complex objects
such as meshes. This class has two Vector values which represent the minimum and maximum. maximum - minimum = length, width height
of the box.

Author : Romesh Selvanathan
Date : 26/02/2012

References:

Ericson, E. (2005). Real-Time Collision Detection. San Francisco: Morgan Kaufmann.

*/ 
#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

#include "BoundingShape.h"
#include "GameUtil.h"

class BoundingBox : BoundingShape
{
public:
	/* Constructors */ 
	BoundingBox() : BoundingShape("BoundingBox")
	{
		min = max = min_surround = max_surround = oldPos = Vec3(0, 0, 0);
	}
	BoundingBox(SmartPtr<Entity> o, Vec3 _min, Vec3 _max) : BoundingShape("BoundingBox", o)
	{
		// min_surround and max_surround make sure used here for game objects 
	    oldPos = o->GetPosition();
		min_surround = _min;
		max_surround = _max;
		min = min_surround + oldPos;
		max = max_surround + oldPos;
	}
	BoundingBox(Vec3 _min, Vec3 _max) : BoundingShape("BoundingBox")
	{
		// if no game object is supplied then the box will be static so apply the positions directly to min and max
		min = _min; max = _max; oldPos = Vec3(0, 0, 0);
	}

	// Update the position of the bounding box if the object moves
	void UpdateBoundingBox()
	{
		if(oldPos != mEntity->GetPosition()){
			oldPos = mEntity->GetPosition();
			min = min_surround + oldPos;
			max = max_surround + oldPos;
		}
	}

	/* Return the closest point from the point that is trying to be compared to */
	Vec3 GetClosesPoint(Vec3 point)
	{
		// The vector to be returned
		Vec3 closest;

		// If point x is less than the minimum x of the AABB
		if(point.x < min.x)
			closest.x = min.x;
		// If point x is greater than the maximum x of the AABB
		else if(point.x > max.x)
			closest.x = max.x;
		// Otherwise it is it inbetween the maximum and minimum range of x
		else
			closest.x = point.x;

		// If point x is less than the minimum y of the AABB
		if(point.y < min.y)
			closest.y = min.y;
		// If point x is greater than the maximum y of the AABB
		else if(point.y > max.y)
			closest.y = max.y;
		// Otherwise it is it inbetween the maximum and minimum range of y
		else
			closest.y = point.y;

		// If point x is less than the minimum z of the AABB
		if(point.z < min.z)
			closest.z = min.z;
		// If point x is greater than the maximum z of the AABB
		else if(point.z > max.z)
			closest.z = max.z;
		// Otherwise it is it inbetween the maximum and minimum range of z
		else
			closest.z = point.z;

		// Return the closest point
		return closest;
	}

	/*
	Checks if a sphere is contained inside the AABB box
	Used for Octree 
	*/
	bool ContainsSphere(Vec3 position, float radius) {
		// Obtain the position and the radius
		Vec3 pos = position;
		float r = radius;
		// Return whether the box contains the sphere within it
		return ((pos.x + radius) > min.x && (pos.x + radius) < max.x &&
				(pos.y + radius) > min.y && (pos.y + radius) < max.y &&
				(pos.z + radius) > min.z && (pos.z + radius) < max.z);
	}

	/* CHeck if spehere has collided with an AABB */ 
	bool CollideWithSphere(Vec3 position, float radius){
		// Obtain the position and the radius of the sphere
		Vec3 pos = position;
		float r = radius;
		// Obtain the the closest side to where the point is
		Vec3 closest = GetClosesPoint(pos);
		// calculate the distance from that point
		Vec3 distanceVec = pos - closest;
		float distance = distanceVec.Length();
		// Check whether distance squared is less than the radius squared if true that means
		// there is a collision
		return ((distance*distance) < (radius*radius));
	}

	/* Set And Get MIN, MAX Vectors */ 
	void SetMinMax(Vec3 _min, Vec3 _max) {min = _min; max = _max;}
	Vec3 GetMin() {return min;}
	Vec3 GetMax() {return max;}
	float GetTest() {return test;}

	/* Debugging */ 
	void Render()
	{
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();

		glColor3f(0, 1, 0);
		glBegin(GL_LINE_LOOP);
			glVertex3f(max.x, max.y, min.z);
			glVertex3f(min.x, max.y, min.z);
			glVertex3f(min.x, min.y, min.z);
			glVertex3f(max.x, min.y, min.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glVertex3f(max.x, min.y, max.z);
			glVertex3f(max.x, max.y, max.z);
			glVertex3f(min.x, max.y, max.z);
			glVertex3f(min.x, min.y, max.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glVertex3f(max.x, max.y, min.z);
			glVertex3f(max.x, max.y, max.z);
			glVertex3f(min.x, max.y, max.z);
			glVertex3f(min.x, max.y, min.z);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glVertex3f(max.x, min.y, max.z);
			glVertex3f(min.x, min.y, max.z);
			glVertex3f(min.x, min.y, min.z);
			glVertex3f(max.x, min.y, min.z);
		glEnd();

		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
	}
private:
	Vec3 min_surround , max_surround; // The min and max constant values that will be added/subtracted to the position to create a box around the object
	Vec3 min, max;			// Min, Max Vectors for the box
	Vec3 oldPos;			// The Old Position of the current entity (Used to check whether the entity has moved or not so that the min and max can be updated)
	float test;				// Debugging
};

#endif 