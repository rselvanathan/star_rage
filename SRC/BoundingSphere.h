/*
Child class of BoundingShape. This class is the bounding sphere that will test its collision with other Bounding Shapes.
Bounding Sphere is a very fast and easy way to determine if two objects have collided as an object is surrounded by
a Sphere. That sphere has a radius which is equal in all directions, so no need to worry about object rotations. This means
that if the distance between the two object positions is less than the two radiuses of the bounding sphere added together 
then a collision has occured.

This class also presents Collision detection with a Bounding box

Author : Romesh Selvanathan

Reference : 

Chris Child , 2nd Year Asteroids Project

Ericson, E. (2005). Real-Time Collision Detection. San Francisco: Morgan Kaufmann.
*/ 
#ifndef __BOUNDINGSPHERE_H__
#define __BOUNDINGSPHERE_H__

#include "GameUtil.h"
#include "GameObjectType.h"
#include "BoundingShape.h"
#include "BoundingBox.h"

class BoundingSphere : public BoundingShape
{
public:
	// Constructor 
	BoundingSphere() : BoundingShape("BoundingSphere"), mRadius(0) 
	{	// Init glu Quadratic's to be used to visually draw the bounding sphere
		quadratic = gluNewQuadric();
		gluQuadricNormals(quadratic, GL_SMOOTH);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
	}

	BoundingSphere(SmartPtr<Entity> o, float r) : BoundingShape("BoundingSphere", o), mRadius(r) 
	{	// Init glu Quadratic's to be used to visually draw the bounding sphere
		quadratic= gluNewQuadric();
		gluQuadricNormals(quadratic, GL_SMOOTH);
		gluQuadricDrawStyle(quadratic, GLU_LINE);
	}
	// Method to test if the collision between two spheres is occuring 
	bool CollisionTest(SmartPtr<BoundingSphere> bs) {
		// Obtain the center of this sphere	
		Vec3 pos1 = GetEntity()->GetPosition();
		// Obtain the center of the other object
		Vec3 pos2 = bs->GetEntity()->GetPosition();
		// Work out the distance squared between the two center positions
		Vec3 temp = (pos2 - pos1);
		float distancesquared = temp.Dot(temp);
		// Obtain the total radii of both spheres
		float radiSum = GetRadius() + bs->GetRadius();
		// Collision occurs if distance squared is less than radi sum squared
		return (distancesquared <= radiSum * radiSum);
	}

	// Allow the spehere to collide with the AABB box
	bool CollisionWithBox(SmartPtr<BoundingBox> aabb)
	{
		return (aabb->CollideWithSphere(mEntity->GetPosition(), mEntity->GetRadius()));
	}

	/*
	Collision Test to see if this Sphere is colliding with a given Plane. The equation is:
	Distance = DOT(plane.normal, sphere.position) - d (d From Plane equation, which is worked out as 
														d = DOT(plane.vertex , plane.normal)
	So if the distance from the plane is less than the radius than a collision has occured 
	(NOT FINAL - a few bugs)
	This can be used as world boundaries as a plane is infinite in length making sure a player does not go beyond a bound 
	*/
	bool CollisionPlaneTest(Vec3 normal, float d)
	{
		Vec3 position = GetEntity()->GetPosition();
		float distance = (position.Dot(normal)) - d;
		if(abs(distance) < mRadius)
			return true;
		return false;
	}

	// Draw the sphere for debugging 
	void Render()
	{
		glPushMatrix();
			glColor3f(1, 1, 1);
			glTranslatef(GetEntity()->GetPosition().x, GetEntity()->GetPosition().y, GetEntity()->GetPosition().z);
			gluSphere(quadratic, mRadius, 15, 15);
		glPopMatrix();
	}

	void SetRadius(float r) { mRadius = r; }
	float GetRadius() { return mRadius; }

protected:
	float mRadius;				// Radius of Sphere
	GLUquadricObj* quadratic;	// quadratic used to init a glu quadratic used to draw a glu sphere
};

#endif