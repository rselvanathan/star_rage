/*
This Object will dynamically add more shots as they are necessary, without having to create a set amount in the beginning.
I chose to do it this way, as the player may never reach the more difficult waves with 10 + enemies. At this point
50 bullets may be required, however it will be a waste to keep that many objects in memory without use. So instead
the list is empty at the beginning and beging to fill up as more and more bullets are required. So if all the bullets
in the list are currently active then a new one will be added, whenever a client is aksing to shoot.

Author : Romesh Selvanathan
Date : 
*/
#include "ShootManager.h"
#include "GameUtil.h"
#include "GameWorld.h"
#include "Entity.h"
#include "Shoot.h"
#include <algorithm>

const int MAX_SHOOT_OBJECTS = 100;

ShootManager* ShootManager::mShootManager;

/* Constructor */
ShootManager::ShootManager()
{
}

/* Destructor */
ShootManager::~ShootManager()
{
	m_shots.clear();
}

/* Initialize the class */
bool ShootManager::Init()
{
	count = 0;

	// If the shoot list if empty create n number of shoot objects and add them into the container
	// otherwise set any shoot object that is alive as not alive
	if(m_shots.empty())
		for(int i = 0; i < MAX_SHOOT_OBJECTS; i++){
			SmartPtr<Shoot> s = new Shoot("", Vec3(0, 0, 0), Vec3(0, 0, 0), 0, Vec3(1, 1, 1));
			s->SetAlive(false);
			m_shots.push_back(s);
		}
	else
		for(ShootList::iterator it = m_shots.begin(); it != m_shots.end(); it++)
			if((*it)->IsAlive())
				(*it)->SetAlive(false);

	return true;
}

/* Add a Shoot object into the game world */
void ShootManager::AddShot(Entity* &o, char* mType, Vec3 position, Vec3 velocity, int TimeToLive, Vec3 mColor, Matrix4 objectMatrix, float scale)
{
	// If there is a object that is not alive, then set it to alive and set its position type of shot, how long it will live, the color,
	// the calling objects' world matrix and the scale of the shoot object
	for(ShootList::iterator it = m_shots.begin(); it != m_shots.end(); it++)
		if(!(*it)->IsAlive()){
			(*it)->SetAlive(true);
			(*it)->SetGameObjectType(mType);
			(*it)->SetPosition(position);
			(*it)->SetVelocity(velocity);
			(*it)->SetTimeToLive(TimeToLive);
			(*it)->SetColor(mColor.x, mColor.y, mColor.z);
			(*it)->SetScale(scale);
			(*it)->SetPlayerWorldMatrix(objectMatrix);
			(*it)->SetWorldMatrix(objectMatrix);

			o = (*it).GetPtr();

			GameWorld::GetInstance()->AddBlendedGameObject((*it));
			GameWorld::GetInstance()->AddCollisionEntity((*it));

			count ++;
			// Return when one object has been added
			return;
		}

	// If not object is alive then create a new object on demand and add it to the list 
	SmartPtr<Shoot> newShoot = new Shoot(mType, position, velocity, TimeToLive, mColor);
	newShoot->SetAlive(true);
	newShoot->SetPlayerWorldMatrix(objectMatrix);
	newShoot->SetGameObjectType(mType);
	newShoot->SetScale(scale);
	o = newShoot.GetPtr();
	GameWorld::GetInstance()->AddBlendedGameObject(newShoot);
	GameWorld::GetInstance()->AddCollisionEntity(newShoot);
	m_shots.push_back(newShoot);
	count ++;
}

/* Add a Shoot object into the game world */
void ShootManager::AddShot(char* mType, Vec3 position, Vec3 velocity, int TimeToLive, Vec3 mColor, Matrix4 objectMatrix, float scale)
{
	// If there is a object that is not alive, then set it to alive and set its position type of shot, how long it will live, the color,
	// the calling objects' world matrix and the scale of the shoot object
	for(ShootList::iterator it = m_shots.begin(); it != m_shots.end(); it++)
		if(!(*it)->IsAlive()){
			(*it)->SetAlive(true);
			(*it)->SetGameObjectType(mType);
			(*it)->SetPosition(position);
			(*it)->SetVelocity(velocity);
			(*it)->SetTimeToLive(TimeToLive);
			(*it)->SetColor(mColor.x, mColor.y, mColor.z);
			(*it)->SetScale(scale);
			(*it)->SetPlayerWorldMatrix(objectMatrix);

			GameWorld::GetInstance()->AddBlendedGameObject((*it));
			GameWorld::GetInstance()->AddCollisionEntity((*it));

			count ++;
			// Return when one object has been added
			return;
		}

	// If not object is alive then create a new object on demand and add it to the list 
	SmartPtr<Shoot> newShoot = new Shoot(mType, position, velocity, TimeToLive, mColor);
	newShoot->SetAlive(true);
	newShoot->SetPlayerWorldMatrix(objectMatrix);
	newShoot->SetGameObjectType(mType);
	newShoot->SetScale(scale);
	GameWorld::GetInstance()->AddBlendedGameObject(newShoot);
	GameWorld::GetInstance()->AddCollisionEntity(newShoot);
	m_shots.push_back(newShoot);
	count ++;
}

//////////////////////////////////////////////////IGAMEWORLDLISTENER////////////////////////////////////////////////////
void ShootManager::OnObjectRemoved(GameWorld* world, Entity* object)
{
	// If a object that is removed from the gameworld is one of the specified below then find it and set it as "not alive"
	if(object->GetType() == GameObjectType("bullet") || object->GetType() == GameObjectType("enemybullet")){
		ShootList::iterator it = find(m_shots.begin(), m_shots.end()-1, object);
		// this makes sure that the found object is not the last item in the container - if "find" does not find the value given 
		// it will equal the last value to "it" 
		// Reference - http://www.cplusplus.com/reference/algorithm/find/
		if((*it) != object)
			return;
		else 
			(*it)->SetAlive(false);

		count--;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////