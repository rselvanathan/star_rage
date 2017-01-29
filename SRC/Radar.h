/*
// LEGACY
Radar system that shows the objects on the map (so far only asteroids) - It takes the current position of the objects and then updates them relative
to the center of the radar with rotation - (Stil buggy as rotations is fine but then as game goes on starts to go wrong slightly ) NOT FINAL 

Reference : http://nfostergames.com/3DRadarSample.htm

Author : Romesh Selvanathan

Last Edit Date : 22/01/2012 = Added The Textures

// UPDATE: 16/02/2012
Complete change of radar. The whole screen has become the radar system, now what happens is same thing as before to test how far an object is from the spaceship 
and instead of showing it in a little radar, the objects positions are shown on the screen as target boxes or if they are not in view of the player as arrows
pointing where the player should look to see the objects in view - similar to this http://www.4gamer.net/patch/demo/freelancer.jpg
If the Object is an enemy the Arrows/Target Box will be shown regardless of distance
REFERENCES - http://www.gamedev.net/topic/327903-freelancer-style-radar/
		   - My own thread - http://www.gamedev.net/topic/620383-gluproject-odd-problem/

UPDATE 01/03/2012 - scaling of radar targets + powerups added
*/

#ifndef __RADAR_H__
#define __RADAR_H__

#include "GameUtil.h"
#include "Entity.h"
#include "GL2DBox.h"
#include "GLBox.h"
#include "Texture.h"
#include "IGameWorldListener.h"
#include "Frustum.h"
#include <map>

class GameWorld;

// Struct that keep current object and its 2D box eqivalent to be drawn on the radar
struct Point 
{
	SmartPtr<Entity> object;			// THe Object
	GL2DBox Box;						// The 2D box to be drawn on screen
	bool Draw;							// Should it be drawn?					
};

class Radar : public IGameWorldListener
{
public:
	/* Constructor */
	Radar();
	/* Destructor */
	~Radar();

	/* Initialize */
	void Init(float range);
	/* Reset the radar */
	void Reset();

	/* Update the Radar */
	void Update(float dt);
	/* Render the Radar */
	void Render();
	
	/* Set the position of the radar */
	void SetPos(Vec3 player) {m_playerPos = player;}
	/* Set the Player Look At/ Forward vector */
	void SetPlayerLookAt(Vec3 look) {m_playerLookAt = look;}

	/* For debugging */
	float GetTest() {return test2;}

	/* Add Object into the List hence creating a new Point */
	void AddObject(SmartPtr<Entity> lptr) 
	{
		Point* p = new Point();
		p->object = lptr;
		points.push_back(p);
	}

	//////////////////////////////////IGAMEWORLDLISTENER//////////////////////////////
	void OnObjectAdded(GameWorld* world, Entity* object)
	{
		// IF an object added to the game world is one of the specfied below then add a new Point to the list
		if(object->GetType() == GameObjectType("asteroid") || object->GetType() == GameObjectType("bigEnemy") ||
			object->GetType() == GameObjectType("smallenemy") || object->GetType() == GameObjectType("hardenemy") ||
			object->GetType() == GameObjectType("poweruphealth") || object->GetType() == GameObjectType("powerupenergy")){
			Point* p = new Point();
			p->object = object;
			points.push_back(p);
		}
	}
	void OnObjectRemoved(GameWorld* world, Entity* object){
		// Go through the list of points and see which one contains the object to be removed
		// When its found remove that point from the list and return
		// This way has a (O(d)) time complexity so have to figure out a better way (d - where goal point is )
		for(PointList::iterator it = points.begin(); it != points.end(); ++it)
		{
			Entity* o = (*it)->object.GetPtr();
			if(o == object){
				points.remove((*it));
				return;
			}
		}	
	}
	/////////////////////////////////////////////////////////////////////////////////

private:
	Vec3 m_playerPos;					// Player Position
	Vec3 m_playerLookAt;				// Player Look At/ Forward Vector

	float test2;						// Debugging purpose

	typedef list<Point*> PointList;		// List of Points
	PointList points;
};

#endif