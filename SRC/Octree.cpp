#include "Octree.h"
#include "GameUtil.h"
#include "MovingEntity.h"
#include "BoundingBox.h"
#include <algorithm>

/*Constructor*/
Octree::Octree(Octree* parent, int nodeDepth, Vec3 pos, float size)
{
	// all children nullified
	for(int i = 0; i < 8 ; i++)
		m_child[i] = NULL;

	// set up node properties
	m_depth = nodeDepth;
	m_position = pos;
	m_size = size;
	m_count = 0;

	m_parent = parent;

	// create a AABB BoundingBox for this node
	/*m_box = new BoundingBox(Vec3(m_position.x - size , 
		m_position.y - size, m_position.z - size), Vec3(m_position.x + size , 
		m_position.y + size, m_position.z + size));*/
	m_box = new BoundingBox(m_position, Vec3(m_position.x + size , 
		m_position.y + size, m_position.z + size));
}

/*Destructor*/
Octree::~Octree()
{
	// Kill all nodes thrugh recursion and remove objects as well as the bounding box
	DeleteChildNodes();
	mObjects.clear();
	SAFE_DELETE(m_box);
}

/*Add Object into the Octree*/
void Octree::AddObject(SmartPtr<MovingEntity> object)
{
	// Sort the object into the correct node from this node onwards
	PlaceObjectIntoNode(object);

	// If this node does not have children AND the size of objects exceeds the maximum per node AND
	// the max depth has not been reached = split nodes (expand the tree)
	if(m_child[0].GetPtr() == NULL && 
	   mObjects.size() >= MAX_OBJECTS &&
	   m_depth <= MAX_DEPTH){

	   ExpandNode();
	}
}

/*Expand parent nodes to create child nodes*/
void Octree::ExpandNode()
{
	// Store into a temp vector
	list<SmartPtr<GameObject> > tmp = mObjects;

	// Clear the current node
	mObjects.clear();

	// size of the new nodes (always half due to radius of a child is alwyas half)
	float newNodeSize = m_size * 0.5f;

	// Create and position the new nodes - Recursion
	// top-left front
	m_child[0] = new Octree(this, m_depth+1, Vec3(m_position.x,				 m_position.y + newNodeSize, m_position.z), newNodeSize);
	// top-right front
	m_child[1] = new Octree(this, m_depth+1, Vec3(m_position.x+ newNodeSize, m_position.y + newNodeSize, m_position.z), newNodeSize);
	// bottom-left front
	m_child[2] = new Octree(this, m_depth+1, Vec3(m_position.x,				 m_position.y ,				 m_position.z), newNodeSize);
	// bottom-right front
	m_child[3] = new Octree(this, m_depth+1, Vec3(m_position.x+ newNodeSize, m_position.y ,				 m_position.z), newNodeSize);
	// top-left back
	m_child[4] = new Octree(this, m_depth+1, Vec3(m_position.x,				 m_position.y + newNodeSize, m_position.z+ newNodeSize), newNodeSize);
	// top-right back
	m_child[5] = new Octree(this, m_depth+1, Vec3(m_position.x+ newNodeSize, m_position.y + newNodeSize, m_position.z+ newNodeSize), newNodeSize);
	// bottom-left back
	m_child[6] = new Octree(this, m_depth+1, Vec3(m_position.x ,			 m_position.y ,				 m_position.z+ newNodeSize), newNodeSize);
	// bottom-right back
	m_child[7] = new Octree(this, m_depth+1, Vec3(m_position.x+ newNodeSize, m_position.y ,				 m_position.z+ newNodeSize), newNodeSize);

	// loop through each object that this node did not hold
	for(list<SmartPtr<GameObject>>::iterator it = tmp.begin(); it != tmp.end(); ++it){
		// sort it into the correct node
		PlaceObjectIntoNode((*it));
	}
}

/* Place a object into a node */
void Octree::PlaceObjectIntoNode(SmartPtr<MovingEntity> object)
{
	// if this node has children
	if(m_child[0].GetPtr() != NULL)
	{
		// go trhough each chil dto see if it goes into the box
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
		else if(m_box->ContainsSphere(object->GetPosition(), object->GetRadius()))
			mObjects.push_back(object);
	}
	// if we do not have children take it
	else if(m_box->ContainsSphere(object->GetPosition(), object->GetRadius()))
		mObjects.push_back(object);
	else 
		mObjects2.push_back(object);
}

bool Octree::RemoveObject(SmartPtr<MovingEntity> object)
{
	// See if the object is inside the list
	list<SmartPtr<GameObject>>::iterator it = std::find(mObjects.begin(), mObjects.end(), object);

	// if it is
	if(it != mObjects.end())
	{
		// remove it from the node
		mObjects.remove(object);
		// stop recursing
		return true;
	}

	it = std::find(mObjects2.begin(), mObjects2.end(), object);
	// if it is
	if(it != mObjects2.end())
	{
		// remove it from the node
		mObjects2.remove(object);
		// stop recursing
		return true;
	}

	if (m_child[0].GetPtr() != NULL) // if not and this node has children
	{
		// then attempt to remove it from all children , skipping out if it has been located
		if (m_child[0]->RemoveObject(object) ||
			m_child[1]->RemoveObject(object) ||
			m_child[2]->RemoveObject(object) ||
			m_child[3]->RemoveObject(object) ||
			m_child[4]->RemoveObject(object) ||
			m_child[5]->RemoveObject(object) ||
			m_child[6]->RemoveObject(object) ||
			m_child[7]->RemoveObject(object) )
		{
			// If it is in the child nodes, do a count and if the number of objects equals 0
			// delete the children 
			if (GetCount() == 0)
				DeleteChildNodes();

			return true; // stop recursing
		}
	}

	// if we got here then the object is not present in any of the node downwards
	return false;
}

/* Find the total number of objects in this subtree */
int Octree::GetCount()
{
	// get number of objects in this tree
	int count = mObjects.size();

	// if this node has children obtain the total number of objects from each child
	if(m_child[0].GetPtr() != NULL){
		for (int i = 0; i < 8; i++)
			count += m_child[i]->GetCount();
	}

	m_count = count;

	return m_count;
}

/* Delete all the children nodes */
void Octree::DeleteChildNodes()
{
	for (int i = 0; i < 8; i++){
		SAFE_DELETE(m_child[i]);
	}
}

/* Start a recursive function to collide it with this node and all children nodes below it */
void Octree::CollideAll()
{
	for (list<SmartPtr<MovingEntity>>::iterator it = mObjects.begin(); it != mObjects.end(); ++it)
	{
		CollideObjectWithNode((*it));
	}
}

void Octree::CollideObjectWithNode(SmartPtr<MovingEntity> object)
{

}


void Octree::DrawTree()
{
	/*glPushMatrix();

		glDisable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glTranslatef(m_position.x, m_position.y, m_position.z);
		glColor3f(0.0f, 1.0f, 0.0f);

		glBegin(GL_QUADS);
			glVertex3f( m_size, m_size, 0.0f);			// Top Right Of The Quad (Top)
			glVertex3f(-0.0f,		m_size,-0.0f);			// Top Left Of The Quad (Top)
			glVertex3f(-0.0f,		m_size, m_size);			// Bottom Left Of The Quad (Top)
			glVertex3f( m_size, m_size, m_size);			// Bottom Right Of The Quad (Top)
	 
			glVertex3f( m_size,	-0.0f, m_size);			// Top Right Of The Quad (Bottom)
			glVertex3f(-0.0f,		-0.0f, m_size);			// Top Left Of The Quad (Bottom)
			glVertex3f(-0.0f,		-0.0f,-0.0f);			// Bottom Left Of The Quad (Bottom)
			glVertex3f( m_size,	-0.0f,-0.0f);			// Bottom Right Of The Quad (Bottom)
	 
			glVertex3f( m_size, m_size, m_size);			// Top Right Of The Quad (Front)
			glVertex3f(-0.0f,		m_size, m_size);			// Top Left Of The Quad (Front)
			glVertex3f(-0.0f,		-0.0f,		m_size);			// Bottom Left Of The Quad (Front)
			glVertex3f( m_size,	-0.0f,		m_size);			// Bottom Right Of The Quad (Front)
	 
			glVertex3f( m_size,	-0.0f,		-0.0f);			// Bottom Left Of The Quad (Back)
			glVertex3f(-0.0f,		-0.0f,		-0.0f);			// Bottom Right Of The Quad (Back)
			glVertex3f(-0.0f,		m_size,	-0.0f);			// Top Right Of The Quad (Back)
			glVertex3f( m_size, m_size,	-0.0f);			// Top Left Of The Quad (Back)
	 
			glVertex3f(-0.0f,		m_size, m_size);			// Top Right Of The Quad (Left)
			glVertex3f(-0.0f,		m_size,	-0.0f);			// Top Left Of The Quad (Left)
			glVertex3f(-0.0f,		-0.0f,		-0.0f);			// Bottom Left Of The Quad (Left)
			glVertex3f(-0.0f,		-0.0f,		m_size);			// Bottom Right Of The Quad (Left)

	 
			glVertex3f( m_size, m_size,	-0.0f);			// Top Right Of The Quad (Right)
			glVertex3f( m_size, m_size, m_size);			// Top Left Of The Quad (Right)
			glVertex3f( m_size,	-0.0f,		m_size);			// Bottom Left Of The Quad (Right)
			glVertex3f( m_size,	-0.0f,		-0.0f);			// Bottom Right Of The Quad (Right)
		glEnd();

		glEnable(GL_TEXTURE_2D);*/

	m_box->Render();

	glPopMatrix();

	if(m_child[0].GetPtr() != NULL)
	{
		for (int i = 0; i < 8; i++){
			m_child[i]->DrawTree();
		}
	}
}