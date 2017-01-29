/*
Base Class For Bounding shapes such as Bounding sphere to test collisions between them 

Author : Romesh Selvanathan 
Date : 03/05/2012

References : 

Dr Chris Child , 2nd Year Asteroids Coursework
*/ 

#ifndef __BOUNDINGSHAPE_H__
#define __BOUNDINGSHAPE_H__

#include "GameObjectType.h"
#include "Entity.h"

class BoundingShape
{
public:
	// Constructor 
	BoundingShape(char const * const type_name) : mType(type_name) {}
	// Destructor  
	BoundingShape(char const * const type_name, SmartPtr<Entity> o)
		: mType(type_name), mEntity(o) {}

	// Collision Testing Methods
	virtual bool CollisionTest(SmartPtr<BoundingShape> bshape) { return false; }
	virtual bool CollisionPlaneTest(SmartPtr<BoundingShape> bshape) { return false; }
	
	// Return Methods
	const GameObjectType& GetType() const { return mType; }

	// Set Methods
	virtual void SetEntity(SmartPtr<Entity> o) { mEntity = o; }
	virtual SmartPtr<Entity> GetEntity() { return mEntity; }

protected:
	GameObjectType mType;				// Type of Game Object / Entity
	SmartPtr<Entity> mEntity;			// Pointer to the Game Entity this shape belongs to
};

#endif