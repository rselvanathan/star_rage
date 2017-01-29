#include "CollisionManager.h"
#include "Entity.h"
#include "GameUtil.h"

/* This method Updated all collisions with other spheres (For Now) */ 
void CollisionManager::UpdateCollision()
{
	float count = 0.0f;

	// Two temporary vectors
	vector<Entity*> temp1;
	vector<Entity*> temp2;

	// Iterate through all objects and all other objects
	for(EntityList::iterator object = mEntity.begin(); object != mEntity.end(); object++)
	{
		for(EntityList::iterator other = mEntity.begin(); other != mEntity.end(); other++)
		{
			// Test with itself and if its tru move to other object
			if(*object == *other)
				continue;

			// Test Collision if its true then inform its listeners a collision has occured 
			if((*object)->CollisionTest(*other)){
				//(*object)->OnCollision((*other));
				//(*other)->OnCollision((*object));
				// Add the Collided Objects into the vector , this is so that if a object removes itself it will cause an error
				// if I kept the OnCollision here (Pretty much keeping a copy of the objects colliding, before doing anything) 
				temp1.push_back((*object)); temp2.push_back((*other));
			}
			count ++;
		}
	}
	// Go Through the Collided objects and resolve them
	for(int i = 0; i < temp1.size(); i ++){
		temp1[i]->OnCollision(temp2[i]);
		temp2[i]->OnCollision(temp1[i]);
	}

	test = count;
}

/* This method Updated all collisions with spheres and Planes (For Now) */ 
void CollisionManager::UpdatePlaneCollisions()
{
	// Iterate through all objects and all other objects
	for(EntityList::iterator object = mEntity.begin(); object != mEntity.end(); object++)
	{
		for(PlaneList::iterator plane = mPlanes.begin(); plane != mPlanes.end(); plane++)
		{
			// Test Collision if its true then inform its listeners a collision has occured 
			if((*object)->CollisionPlaneTest(*plane)){
				(*object)->OnCollision((*plane));
				(*plane)->OnCollision((*object));
			}
		}
	}
}