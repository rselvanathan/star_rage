/* 
This class will hold all the game objects that will be in the game world and will update and Render them
This class will also update the Collision Detection in the world as well as the Frustum culling class.
It uses the singleton pattern as only one game world can exist at any time.

There Two types of lists in this class, one for transluscent objects and one for blended objects. This is done
to make sure that all the blended objects are rendered after the transluscent ones are rendered. 

Author : Romesh Selvanathan
Date : 03/05/2012
*/ 

#include "GameWorld.h"
#include "CollisionManager.h"
#include "OctreeCollision.h"
#include "MessageDispatcher.h"

GameWorld* GameWorld::mGameWorld;

// Constrctor 
GameWorld::GameWorld()
{
	objectsRendered = 0;
	
	// Create a new Octree
	octree = new OctreeCollision(NULL, 0, 170, Vec3(0, 0, 0));
}

GameWorld::~GameWorld()
{
}

// Reset the Game World
void GameWorld::Reset()
{
	mGameObjects.clear();
	mListeners.clear();

	mCollisions.Reset();
	octree->Reset();
}

// Add a object. I have two methods with different parameters, which are
// moving and normal entity. THis is for the octree as it needs to differentiate
// between what object is being added for the octree to function efficiently. 
void GameWorld::AddGameObject(Entity* lptr)
{
	// Set the Objects Game World as this
	lptr->SetGameWorld(this);
	mGameObjects.push_back(lptr);
	// If object has a bounding box or sphere then add into octree
	if(lptr->GetBoundingBox() || lptr->GetBoundingShape())
		octree->AddObject(lptr);
	// Fire message to all listeners on which object was added
	FireObjectAdded(lptr);
}

// Add a object. I have two methods with different parameters, which are
// moving and normal entity. THis is for the octree as it needs to differentiate
// between what object is being added for the octree to function efficiently. 
void GameWorld::AddGameObject(MovingEntity* lptr)
{
	// Set the Objects Game World as this
	lptr->SetGameWorld(this);
	mGameObjects.push_back(lptr);
	// If object has a bounding box or sphere then add into octree
	if(lptr->GetBoundingBox() || lptr->GetBoundingShape())
		octree->AddObject(lptr);
	// Fire message to all listeners on which object was added
	FireObjectAdded(lptr);
}

// Add a object. I have two methods with different parameters, which are
// moving and normal entity. THis is for the octree as it needs to differentiate
// between what object is being added for the octree to function efficiently. 
void GameWorld::AddBlendedGameObject(Entity* lptr)
{
	lptr->SetGameWorld(this);
	mBlendedGameObjects.push_back(lptr);
	// If object has a bounding box or sphere then add into octree
	if(lptr->GetBoundingBox() || lptr->GetBoundingShape())
		octree->AddObject(lptr);
	// Fire message to all listeners on which object was added
	FireObjectAdded(lptr);
}

// Add a object. I have two methods with different parameters, which are
// moving and normal entity. THis is for the octree as it needs to differentiate
// between what object is being added for the octree to function efficiently. 
void GameWorld::AddBlendedGameObject(MovingEntity* lptr)
{
	lptr->SetGameWorld(this);
	mBlendedGameObjects.push_back(lptr);
	// If object has a bounding box or sphere then add into octree
	if(lptr->GetBoundingBox() || lptr->GetBoundingShape())
		octree->AddObject(lptr);
	// Fire message to all listeners on which object was added
	FireObjectAdded(lptr);
}

// Remove a game object
void GameWorld::RemoveGameObject(Entity* lptr)
{
	// this if makes sure that if object is being removed twice (for example one bullet hit by two enemies)
	// then it does not cause pointer issue where pointer is being removed another time which does not exist in the list anymore
	if(lptr->GetWorld() != NULL) {
		mGameObjects.remove(lptr);
		mCollisions.RemoveCollisionListener(lptr);
		octree->RemoveObject(lptr);
		lptr->SetGameWorld(NULL);
		// Fire to all listeners on which game object was removed
		FireObjectRemoved(lptr);
	}
}

// Remove a game object
void GameWorld::RemoveGameObject(MovingEntity* lptr)
{
	// this if makes sure that if object is being removed twice (for example one bullet hit by two enemies)
	// then it does not cause pointer issue where pointer is being removed another time which does not exist in the list anymore
	if(lptr->GetWorld() != NULL) {
		mGameObjects.remove(lptr);
		mCollisions.RemoveCollisionListener(lptr);
		octree->RemoveObject(lptr);
		lptr->SetGameWorld(NULL);
		// Fire to all listeners on which game object was removed
		FireObjectRemoved(lptr);
	}
}

// Remove a game object
void GameWorld::RemoveBlendedGameObject(Entity* lptr)
{
	// this if makes sure that if object is being removed twice (for example one bullet hit by two enemies)
	// then it does not cause pointer issue where pointer is being removed another time which does not exist in the list anymore
	if(lptr->GetWorld() != NULL) {
		mBlendedGameObjects.remove(lptr);
		mCollisions.RemoveCollisionListener(lptr);
		octree->RemoveObject(lptr);
		lptr->SetGameWorld(NULL);
		// Fire to all listeners on which game object was removed
		FireObjectRemoved(lptr);
	}
}

// Remove a game object
void GameWorld::RemoveBlendedGameObject(MovingEntity* lptr)
{
	// this if makes sure that if object is being removed twice (for example one bullet hit by two enemies)
	// then it does not cause pointer issue where pointer is being removed another time which does not exist in the list anymore
	if(lptr->GetWorld() != NULL) {
		mBlendedGameObjects.remove(lptr);
		mCollisions.RemoveCollisionListener(lptr);
		octree->RemoveObject(lptr);
		lptr->SetGameWorld(NULL);
		// Fire to all listeners on which game object was removed
		FireObjectRemoved(lptr);
	}
}

// Update the Game World
void GameWorld::Update(float dt)
{
	// Update all the MessageDispatcher Delayed Messages
	MessageDispatcher::GetInstance()->DispatchDelayedMessages();
	// Do the Octree Collisions
	octree->Update();
	// Update all Opaque objects
	GameObjectList::iterator object = mGameObjects.begin();
	while(object != mGameObjects.end()){
		// keep a copy just in case a object is deleted
		GameObjectList::iterator copy = object++;
		(*copy)->Update(dt);
	}
	// Update all the blending Objects
	GameObjectList::iterator object_blend = mBlendedGameObjects.begin();
	while(object_blend != mBlendedGameObjects.end()){
		// keep a copy just in case a object is deleted
		GameObjectList::iterator copy = object_blend++;
		(*copy)->Update(dt);
	}
}

// Render the Game World
void GameWorld::Render()
{
	// Update the frustum 
	frustum.UpdateFrustum();

	objectsRendered = 0; // Set how many objects rendered to 0 - Testing purposes

	// Render all Opaque objects
	for(GameObjectList::iterator object = mGameObjects.begin(); object != mGameObjects.end(); object++)
	{
		// Obtain the current object positions 
		float x,y,z;
		x = (*object)->GetPosition().x;
		y = (*object)->GetPosition().y;
		z = (*object)->GetPosition().z;
		// If the Object is not in the frustum, then don't render it
		if(frustum.SphereInFrustum(x, y, z, (*object)->GetRadius())) {
			(*object)->PreRender();
			(*object)->Render();
			(*object)->PostRender();
			objectsRendered++;
		}
	}
	// Render all Blended Objects
	for(GameObjectList::iterator object = mBlendedGameObjects.begin(); object != mBlendedGameObjects.end(); object++)
	{
		// Obtain the current object positions 
		float x,y,z;
		x = (*object)->GetPosition().x;
		y = (*object)->GetPosition().y;
		z = (*object)->GetPosition().z;
		// If the Object is not in the frustum, then don't render it
		if(frustum.SphereInFrustum(x, y, z, (*object)->GetRadius())) {
			(*object)->PreRender();
			(*object)->Render();
			(*object)->PostRender();
			objectsRendered++;
		}
	}
}
