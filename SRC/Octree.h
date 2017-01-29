#ifndef __OCTREE_H__
#define __OCTREE_H__

#include "GameUtil.h"
#include "MovingEntity.h"
#include "BoundingBox.h"

#define MAX_OBJECTS 8
#define MAX_DEPTH 5

class Octree
{
public:
	Octree(Octree* parent, int nodeDepth, Vec3 pos, float size);
	~Octree();

	void CleanTree();
	void DeleteChildNodes();
	void RebuildTree();

	void Update();
	void Render(); // Use for debugging

	void AddObject(SmartPtr<MovingEntity> object);
	bool RemoveObject(SmartPtr<MovingEntity> object);

	void ExpandNode();
	void PlaceObjectIntoNode(SmartPtr<MovingEntity> object);
	void RepositionObjects();

	void CollideAll();
	void CollideObjectWithNode(SmartPtr<MovingEntity> object);

	void DrawTree();

	SmartPtr<BoundingBox> GetBoundingBox() {return m_box;}
	int GetCount();

private:
	SmartPtr<Octree> m_parent;		// Pointer to parent node
	SmartPtr<Octree> m_child[8];	// Array of Children nodes
	int m_depth;					// maximum depth of the tree
	Vec3 m_position;			// the position of the node
	float m_size;					// side of the box
	SmartPtr<BoundingBox> m_box;				// the Axis Aligned Bounding Box

	int m_count;

	typedef list<SmartPtr<MovingEntity>> GameObjectList;
	GameObjectList mObjects;
	GameObjectList mObjects2;
};

#endif