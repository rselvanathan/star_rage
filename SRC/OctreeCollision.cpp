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
#include "OctreeCollision.h"
#include "BoundingBox.h"
#include "GameUtil.h"
#include "Entity.h"
#include "MovingEntity.h"

// constant maximum depth of the tree to 1
const int MAX_DEPTH = 1;

// Constructor
OctreeCollision::OctreeCollision(OctreeCollision* parent, int Depth, float Size, Vec3 pos)
{
	size = Size;
	depth = Depth;

	mPosition = pos;

	p_boundingbox = new BoundingBox(Vec3(mPosition.x - size, mPosition.y - size, mPosition.z - size), 
		Vec3(mPosition.x + size, mPosition.y + size, mPosition.z + size));

	float newNodeSize = size * 0.5f;

	m_parent = parent;

	if(Depth < MAX_DEPTH){
		// Top left
		m_child[0] = new OctreeCollision(this, depth + 1, newNodeSize, 
			Vec3(mPosition.x + newNodeSize, mPosition.y + newNodeSize, mPosition.z + newNodeSize));
		// Top right
		m_child[1] = new OctreeCollision(this,depth + 1, newNodeSize, 
			Vec3(mPosition.x - newNodeSize, mPosition.y + newNodeSize, mPosition.z + newNodeSize));
		// Top bottom Left
		m_child[2] = new OctreeCollision(this,depth + 1, newNodeSize, 
			Vec3(mPosition.x + newNodeSize, mPosition.y + newNodeSize, mPosition.z - newNodeSize));
		// Top Bottom Right
		m_child[3] = new OctreeCollision(this,depth + 1, newNodeSize, 
			Vec3(mPosition.x - newNodeSize, mPosition.y + newNodeSize, mPosition.z - newNodeSize));
		// Bottom Left
		m_child[4] = new OctreeCollision(this,depth + 1, newNodeSize, 
			Vec3(mPosition.x + newNodeSize, mPosition.y - newNodeSize, mPosition.z + newNodeSize));
		// Bottom Right
		m_child[5] = new OctreeCollision(this,depth + 1, newNodeSize, 
			Vec3(mPosition.x - newNodeSize, mPosition.y - newNodeSize, mPosition.z + newNodeSize));
		// Bottom Bottom Left
		m_child[6] = new OctreeCollision(this,depth + 1, newNodeSize,
			Vec3(mPosition.x + newNodeSize, mPosition.y - newNodeSize, mPosition.z - newNodeSize));
		// Bottom Bottom Right
		m_child[7] = new OctreeCollision(this,depth + 1, newNodeSize, 
			Vec3(mPosition.x - newNodeSize, mPosition.y - newNodeSize, mPosition.z - newNodeSize));
	}
}

// Destructor
OctreeCollision::~OctreeCollision()
{
	// Clear the objects and delete the children
	if(!m_objects.empty())
		m_objects.clear();
	if(!m_movingObjects.empty())
		m_movingObjects.clear();

	if(depth < MAX_DEPTH) {
		for(int i = 0; i < 8; i ++) {
			SAFE_DELETE(m_child[i]);
		}
	}
}

// Reset the Octree, clearing everything
void OctreeCollision::Reset()
{
	// Clear the objects and reset the children
	if(!m_objects.empty())
		m_objects.clear();
	if(!m_movingObjects.empty())
		m_movingObjects.clear();

	if(depth < MAX_DEPTH) {
		for(int i = 0; i < 8; i ++) {
			m_child[i]->Reset();
		}
	}
}

// Add object into the tree, which is recursive, that continously checks whether a child node constains it
// first before checking itself, then it will add the object appropriatly 
void OctreeCollision::AddObject(Entity* object)
{
	// if this node has children
	if(depth < MAX_DEPTH)
	{
		// go through each child to see if it goes into the box
		if(m_child[0]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[0]->AddObject(object);
		else if(m_child[1]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[1]->AddObject(object);
		else if(m_child[2]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[2]->AddObject(object);
		else if(m_child[3]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[3]->AddObject(object);
		else if(m_child[4]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[4]->AddObject(object);
		else if(m_child[5]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[5]->AddObject(object);
		else if(m_child[6]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[6]->AddObject(object);
		else if(m_child[7]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[7]->AddObject(object);
		// or accept it into our own box
		else if(p_boundingbox->ContainsSphere(object->GetPosition(), object->GetRadius()))
		{
			m_objects.push_back(object);
		}
	// If this node does nto have children then accept it in this box, it it is contained
	} else
		if(p_boundingbox->ContainsSphere(object->GetPosition(), object->GetRadius()))
		{
			m_objects.push_back(object);
		}
}

// Add object into the tree, which is recursive, that continously checks whether a child node constains it
// first before checking itself, then it will add the object appropriatly 
void OctreeCollision::AddObject(MovingEntity* object)
{
	// if this node has children
	if(depth < MAX_DEPTH)
	{
		// go through each child to see if it goes into the box
		if(m_child[0]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[0]->AddObject(object);
		else if(m_child[1]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[1]->AddObject(object);
		else if(m_child[2]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[2]->AddObject(object);
		else if(m_child[3]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[3]->AddObject(object);
		else if(m_child[4]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[4]->AddObject(object);
		else if(m_child[5]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[5]->AddObject(object);
		else if(m_child[6]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[6]->AddObject(object);
		else if(m_child[7]->GetBoundingBox()->ContainsSphere(object->GetPosition(), object->GetRadius()))
			m_child[7]->AddObject(object);
		// or accept it into our own box
		else if(p_boundingbox->ContainsSphere(object->GetPosition(), object->GetRadius()))
		{
			m_movingObjects.push_back(object);
		}
	// If there are no children then accept it in the root node as long it is withing the world bounds
	} else
		if(p_boundingbox->ContainsSphere(object->GetPosition(), object->GetRadius()))
		{
			m_movingObjects.push_back(object);
		}
}

// Recursive function, that will remove an object, by first checking if the object is within the current node,
// if not then it ill check if it is within its children until the object is found
bool OctreeCollision::RemoveObject(Entity* object)
{
	// First try find the static object in this node
	for(List::iterator it = m_objects.begin(); it != m_objects.end(); it ++){
		if(object == (*it)){
			m_objects.remove((*it));
			// If Found come out of the method
			return true;
		}
	}

	// If this node has children find the the object in one of those
	// IF it found in one of the children nodes then jump out of the method
	if(depth < MAX_DEPTH) {
		if(m_child[0]->RemoveObject(object))
			return true ;
		else if(m_child[1]->RemoveObject(object))
			return true ;
		else if(m_child[2]->RemoveObject(object))
			return true ;
		else if(m_child[3]->RemoveObject(object))
			return true ;
		else if(m_child[4]->RemoveObject(object))
			return true ;
		else if(m_child[5]->RemoveObject(object))
			return true ;
		else if(m_child[6]->RemoveObject(object))
			return true ;
		else if(m_child[7]->RemoveObject(object))
			return true ;
	}
	// the object does not exist in this or any of the children nodes
	else
		return false;
}

// Recursive function, that will remove an object, by first checking if the object is within the current node,
// if not then it ill check if it is within its children until the object is found
bool OctreeCollision::RemoveObject(MovingEntity* object)
{
	// First try find the moving object in this node
	for(MovingList::iterator it = m_movingObjects.begin(); it != m_movingObjects.end(); it ++){
		if(object == (*it)){
			m_movingObjects.remove((*it));
			// If Found come out of the method
			return true;
		}
	}

	// If this node has children find the the object in one of those
	// IF it found in one of the children nodes then jump out of the method
	if(depth < MAX_DEPTH) {
		if(m_child[0]->RemoveObject(object))
			return true ;
		else if(m_child[1]->RemoveObject(object))
			return true ;
		else if(m_child[2]->RemoveObject(object))
			return true ;
		else if(m_child[3]->RemoveObject(object))
			return true ;
		else if(m_child[4]->RemoveObject(object))
			return true ;
		else if(m_child[5]->RemoveObject(object))
			return true ;
		else if(m_child[6]->RemoveObject(object))
			return true ;
		else if(m_child[7]->RemoveObject(object))
			return true ;
	}
	// the object does not exist in this or any of the children nodes
	else
		return false;
}

// A recursive function that will allow the tree to be traversed back from the child to the root again
// with the object in interest
void OctreeCollision::RootTraversal(Entity* object)
{
	// IF this node does have a parent recursivly go up the tree
	if(m_parent != NULL)
		m_parent->RootTraversal(object);
	// If this is the root then add the Object 
	else
		AddObject(object);
}

// A recursive function that will allow the tree to be traversed back from the child to the root again
// with the object in interest
void OctreeCollision::RootTraversal(MovingEntity* object)
{
	// IF this node does have a parent recursivly go up the tree
	if(m_parent != NULL)
		m_parent->RootTraversal(object);
	// If this is the root then add the Object 
	else
		AddObject(object);
}

// Collide with Objects in Parent node and when this objec does collide with another then add those into the two lists 
// that belongs to the parent 
// NOTE : If this method had its own two temporary lists rather than using the parent's then if the object is removed here
// then the game will crash - reason being when the method exits and enters the parent's collision loop again, at that point the object
// that was provided as a pointer here could have been removed. THis means that there would be a null pointer at the parent, which in turn means
// it cannot iterate further as this object would be missing
void OctreeCollision::CollideWithParentNode(Entity* o, vector<Entity*> &temp, vector<Entity*> &temp2)
{
	for(MovingList::iterator object2 = m_movingObjects.begin(); object2 != m_movingObjects.end(); object2++){
		if(o == (*object2))
			continue;

		if(o->CollisionTest((*object2))){
			temp.push_back(o);
			temp2.push_back((*object2));
		}
	}
}

// Collide with Objects in Parent node and when this objec does collide with another then add those into the two lists 
// that belongs to the parent 
// NOTE : If this method had its own two temporary lists rather than using the parent's then if the object is removed here
// then the game will crash - reason being when the method exits and enters the parent's collision loop again, at that point the object
// that was provided as a pointer here could have been removed. THis means that there would be a null pointer at the parent, which in turn means
// it cannot iterate further as this object would be missing
void OctreeCollision::CollideWithParentNode(MovingEntity* o, vector<Entity*> &temp, vector<Entity*> &temp2)
{
	// Test the given object with every moving entity in this child
	for(MovingList::iterator object2 = m_movingObjects.begin(); object2 != m_movingObjects.end(); object2++){
		if(o == (*object2))
			continue;

		if(o->CollisionTest((*object2))){
			temp.push_back(o);
			temp2.push_back((*object2));
		}
	}

	// Test object with every static object in this child
	for(List::iterator object2 = m_objects.begin(); object2 != m_objects.end(); object2++){
		if(o == (*object2))
			continue;

		if(o->CollisionTest((*object2))){
			temp.push_back(o);
			temp2.push_back((*object2));
		}
	}
}

// Update the Octree at every frame update
void OctreeCollision::Update()
{
	// A temporary vector used to remove the Moving Entities that have entered
	// a new subtree
	vector<MovingEntity*> removal;

	// Two Temporary vectors used to keep a list of the pairs of objects that have collided and
	// resolve them together after the collision tests are over. This is done, because if the
	// collisions are resolved immediatly then one of the Objects can be removed. This means that
	// a Null pointer will be created during the iteration/Collision Test phase as moving to the
	// next item in the list during iteration requires a valid current item.
	vector<Entity*> temp;
	vector<Entity*> temp2;
	
	// For Every Moving Object
	for(MovingList::iterator object = m_movingObjects.begin(); object != m_movingObjects.end(); object++){
		// Does this Bounding Box of this node contain the object?
		if(p_boundingbox->ContainsSphere((*object)->GetPosition(), (*object)->GetRadius())){
			// if Yes, then for every moving entity in this node
			for(MovingList::iterator object2 = m_movingObjects.begin(); object2 != m_movingObjects.end(); object2++){
				// check Whether the objects are the same
				if((*object)==(*object2))
					// if yes then move onto the next object
					continue;

				// if the objects are in collision then store them in the 
				// two temporary vectors to be resolved later
				if((*object)->CollisionTest((*object2))){
					temp.push_back((*object));
					temp2.push_back((*object2));
				}
			}

			// Now test that same moving object with any static objects in the node. 
			for(List::iterator object2 = m_objects.begin(); object2 != m_objects.end(); object2++){
				// Safety check first
				if((*object)==(*object2))
					continue;

				// if the objects are in collision then store them in the 
				// two temporary vectors to be resolved later
				if((*object)->CollisionTest((*object2))){
					temp.push_back((*object));
					temp2.push_back((*object2));
				}
			}
			// Collide this object will all the children nodes by also passing in the temporary vectors
			// NOTE : Passing in the two temprary vectors that will resolve the collision, because if the object passed in here,
			// were to be removed by the child then the object would be a null pointer, so this means the game will crash as you 
			// cannot iterate further from here. SO passing in the temprary vectors in this method, makes sure that only AFTER leaving
			// this "for" loop shall the object be resolved and not inside the child
			for(int i = 0; i < 8; i ++)
				if(depth < MAX_DEPTH)
					m_child[i]->CollideWithParentNode((*object), temp, temp2);
		// If the bounding box of this node does not contain the object then store in the removal list
		// to be removed later
		} else 
			removal.push_back((*object));
	}

	// If this node has children then
	if(depth < MAX_DEPTH){
		// Collide static objects in this node with its children
		for(List::iterator object = m_objects.begin(); object != m_objects.end(); object++){
			for(int i = 0; i < 8; i ++)
					m_child[i]->CollideWithParentNode((*object), temp, temp2);
		}	
	}

	// Now remove every moving entity in this node that has moved when the test occured
	// And then Then traverse from the root node to find the current node the object is in currently
	for(vector<MovingEntity*>::iterator it = removal.begin(); it != removal.end(); it++){
		// Remove the object
		RemoveObject(*it);
		// Traverse from the Root to find the right node in which the object belongs to
		RootTraversal(*it);
	}

	// Resolve all the collision pairs
	for(int i = 0; i < temp.size(); i ++) {
		temp[i]->OnCollision(temp2[i]);
		temp2[i]->OnCollision(temp[i]);
	}

	// Update all the Children with their collision Tests
	if(depth < MAX_DEPTH) {
		for(int i = 0; i < 8; i ++) {
			m_child[i]->Update();
		}
	}

}

// Debugging
float OctreeCollision::GetCount()
{
	int temptest = m_objects.size();

	if(depth < MAX_DEPTH)
	{
		for(int i = 0; i < 8; i ++)
			temptest += m_child[i]->GetCount();
	}

	test = temptest;

	return test;
}

// Debugging
void OctreeCollision::Render()
{
	p_boundingbox->Render();

	if(depth < MAX_DEPTH) {
		for(int i = 0; i < 8; i ++)
			m_child[i]->Render();
	}
}