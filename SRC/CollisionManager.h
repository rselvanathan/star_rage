/*
This is a Collision Manager class that will manage and update all the collisions, by Brute Force

*CURRENTLY NOT USED, BUT HERE FOR SHOW*

Author : Romesh Selvanathan
*/
#ifndef __COLLISIONMANAGER_H__
#define __COLLISIONMANAGER_H__

//#include "GameUtil.h"
#include "Entity.h"

class CollisionManager
{
public:
	// Constructor
	CollisionManager(){test = 0;}
	// Destructor 
	~CollisionManager(){}

	void Reset() {mEntity.clear();}
	float GetTest() {return test;}

	// Update all the collisions 
	void UpdateCollision();
	void UpdatePlaneCollisions();

	// Add, Remove and Return the Collision Entities 
	void AddCollisionListener( Entity* lptr){mEntity.push_back(lptr);}
	void RemoveCollisionListener(Entity* lptr){mEntity.remove(lptr);}
	//list< SmartPtr< Entity > > GetCollisionListener(){return mEntity;}

	// Same as above But for all the planes ( NOT FINAL ) 
	void AddPlaneListener( Entity* lptr){mPlanes.push_back(lptr);}
	void RemovePlaneListener(Entity* lptr){mPlanes.remove(lptr);}
	//list< SmartPtr< Entity > > GetPlaneListeners(){return mPlanes;}

private:
	typedef list< Entity* > EntityList;			
	EntityList mEntity;								// List of all Collision Enitities 

	typedef list< Entity* > PlaneList;
	PlaneList mPlanes;								// List of all Collision Planes

	float test;
};

#endif 