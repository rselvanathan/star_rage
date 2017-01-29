/*
This class creates an octree to provide collision detection, for a more efficient way of doing it
compared to the brute force method. This octree has static 8 shildren with a single root. However
it can have more than one hierarchy, However for the current game this is built for, it is more effiecient
to only create one. Each child node has its own list of both moving and static entities, which it will test
with other objects that are in the same or root node. 

Two different lists are kept, for both moving and static objects to make sure that there is a difference between them.
This is because static object and static objects should not collide with each other, so having all the objects
under one list would be wasteful.

Author : Romesh Selvanathan
Date : 03/05/2012
*/
#ifndef __OCTREECOLLISION_H__
#define __OCTREECOLLISION_H__

#include"GameUtil.h"

class BoundingBox;
class Entity;
class MovingEntity;

class OctreeCollision
{
public:
	// Constructor
	OctreeCollision(OctreeCollision* parent, int Depth, float Size, Vec3 pos);
	// Destructor
	~OctreeCollision();
	// Reset the Octree, clearing everything
	void Reset();
	// Update the Octree at every frame update
	void Update();
	// Render for debugging
	void Render();

	// A recursive function that will allow the tree to be traversed back from the child to the root again
	// with the object in interest
	void RootTraversal(Entity* o);
	void RootTraversal(MovingEntity* o);
	// Add object into the tree, which is recursive, that continously checks whether a child node constains it
	// first before checking itself, then it will add the object appropriatly 
	void AddObject(Entity* o);
	void AddObject(MovingEntity* o);
	// Recursive function, that will remove an object, by first checking if the object is within the current node,
	// if not then it ill check if it is within its children until the object is found
	bool RemoveObject(Entity* o);
	bool RemoveObject(MovingEntity* o);
	// Collide with Objects in Parent node. Both the temoriary vectors are also passed in to 
	// avoid problems, as collisions should only be resolved AFTER finishing the collision tests
	// and not before, which needs to be done at the parent node.
	void CollideWithParentNode(Entity* o, vector<Entity*> &temp, vector<Entity*> &temp2);
	void CollideWithParentNode(MovingEntity* o, vector<Entity*> &temp, vector<Entity*> &temp2);

	/* Debugging */
	SmartPtr<BoundingBox> GetBoundingBox() {return p_boundingbox;}
	float GetCount();
	float GetTest() {return test;}
private:
	SmartPtr<BoundingBox> p_boundingbox;		// The bounding box used to test whether an object is contained
		
	typedef list<Entity*> List;
	List m_objects;								// The list of static objects

	typedef list<MovingEntity*> MovingList;
	MovingList m_movingObjects;					// THe list of moving objects

	OctreeCollision* m_child[8];				// Pointers to the 8 children of this node
	OctreeCollision* m_parent;					// A pointer to the parent of this node

	Vec3 mPosition;								// The center position of the node, relative to the world

	float size;									// size of each node
	int depth;									// how deep the node is, i.e two hierarchies or one
	float test;									// debugging
};

#endif