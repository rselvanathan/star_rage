/*
This Object will dynamically add more shots as they are necessary, without having to create a set amount in the beginning.
I chose to do it this way, as the player may never reach the more difficult waves with 10 + enemies. At this point
50 bullets may be required, however it will be a waste to keep that many objects in memory without use. So instead
the list is empty at the beginning and beging to fill up as more and more bullets are required. So if all the bullets
in the list are currently active then a new one will be added, whenever a client is aksing to shoot.

Author : Romesh Selvanathan
Date : 
*/
#ifndef __SHOOTMANAGER_H__
#define __SHOOTMANAGER_H__

#include "GameUtil.h"
#include "IGameWorldListener.h"

class Shoot;
class Entity;

class ShootManager : public IGameWorldListener
{
public:
	static ShootManager* GetInstance()
	{
		if(mShootManager == NULL)
			mShootManager = new ShootManager();
		return mShootManager;
	} 
	/* Destructor */
	~ShootManager();
	/* Reset the Manager */
	bool Init();
	/* Add a Shoot object into the game world */
	void AddShot(Entity* &o, char* mType, Vec3 position, Vec3 velocity, int TimeToLive, Vec3 mColor, Matrix4 objectMatrix, float scale);
	void AddShot(char* mType, Vec3 position, Vec3 velocity, int TimeToLive, Vec3 mColor, Matrix4 objectMatrix, float scale);

	//////////////////////////////////////////////////IGAMEWORLDLISTENER////////////////////////////////////////////////////
	void OnObjectAdded(GameWorld* world, Entity* object){}
	void OnObjectRemoved(GameWorld* world, Entity* object);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Debugging */
	int GetCount() {return count;}

private:
	static ShootManager* mShootManager;
	/* Constructor */
	ShootManager();

	typedef vector<SmartPtr<Shoot>> ShootList;
	ShootList m_shots;								// List of Shots

	int count;										// Debugging
};

#endif