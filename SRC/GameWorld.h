/* 
This class will hold all the game objects that will be in the game world and will update and Render them
This class will also update the Collision Detection in the world as well as the Frustum culling class.
It uses the singleton pattern as only one game world can exist at any time.

There Two types of lists in this class, one for transluscent objects and one for blended objects. This is done
to make sure that all the blended objects are rendered after the transluscent ones are rendered. 

Author : Romesh Selvanathan
Date : 03/05/2012
*/ 

#ifndef __GAMEWORLD_H__
#define __GAMEWORLD_H__

#include "CollisionManager.h"
#include "IGameWorldListener.h"
#include "Entity.h"
#include "MovingEntity.h"
#include <list>
#include "SmartPtr.h"
#include "Frustum.h"

#include "OctreeCollision.h"

class GameWorld
{
public:
	// Obtain a instance from the class
	static GameWorld* GetInstance()
	{
		if(mGameWorld == NULL)
			mGameWorld = new GameWorld();
		return mGameWorld;
	} 
	// Destructor
	~GameWorld();
	// Reset the Game world clearing everything
	void Reset();
	// Update the Game World
	void Update(float dt);
	// Render the Game World
	void Render();
	// Add a object. I have two methods with different parameters, which are
	// moving and normal entity. THis is for the octree as it needs to differentiate
	// between what object is being added for the octree to function efficiently. 
	void AddGameObject(Entity* lptr);
	void AddGameObject(MovingEntity* lptr);
	// Add a Blended Object
	void AddBlendedGameObject(Entity* lptr);
	void AddBlendedGameObject(MovingEntity* lptr);
	// Remove a game object
	void RemoveGameObject(Entity* lptr);
	void RemoveGameObject(MovingEntity* lptr);
	// Remove a blended object 
	void RemoveBlendedGameObject(Entity* lptr);
	void RemoveBlendedGameObject(MovingEntity* lptr);

	// Accessor methods
	const int GetObjectsRendered() const{return objectsRendered;}
	SmartPtr<Frustum> GetFrustum() {return &frustum;}
	SmartPtr<OctreeCollision> GetOctree() {return octree;}
	CollisionManager GetCollisionManager() {return mCollisions;}
	list<Entity*> GetGameObjects() const {return mGameObjects;}

	/* Add listener */
	void AddListener( IGameWorldListener* lptr) { mListeners.push_back(lptr); }
	/* Remove listener */
	void RemoveListener( IGameWorldListener* lptr) { mListeners.remove(lptr); }
	/* Send Messages to all listeners */
	void FireObjectAdded(Entity* ptr) {
		for (GameWorldListenerList::iterator it = mListeners.begin(); it != mListeners.end(); ++it)
			(*it)->OnObjectAdded(this, ptr);
	}
	// Fire when an even when object has been removed
	void FireObjectRemoved(Entity* ptr){
		for (GameWorldListenerList::iterator it = mListeners.begin(); it != mListeners.end(); ++it)
			(*it)->OnObjectRemoved(this, ptr);
	}

	// LEGACY - not used
	void AddCollisionEntity(Entity* lptr) {mCollisions.AddCollisionListener(lptr);}
	void AddPlaneEntity(SmartPtr<Entity> lptr) {mCollisions.AddPlaneListener(lptr);}

private:
	static GameWorld* mGameWorld;		// Singelton pointer
	GameWorld();						// Constructor

	CollisionManager mCollisions;		// LEGACY - old brute force collision manager
	SmartPtr<OctreeCollision> octree;	// Octree Collision Manager

	Frustum frustum;					// Used for frustum culling
	int objectsRendered;				// To sho whow many objects are rendered testing;

	typedef list<IGameWorldListener*> GameWorldListenerList;
	GameWorldListenerList mListeners;	// List of listeners
	
	typedef list< Entity* > GameObjectList;
	GameObjectList mGameObjects;		// List of Transluscent game objects
	GameObjectList mBlendedGameObjects; // List of blended game objects 
};

#endif 