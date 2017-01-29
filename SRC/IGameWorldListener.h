/*
An interface class that allows objects to listen to updates from the game world

Author : Romesh Selvanathan
Date : 03/05/12
*/

#ifndef __IGAMEWORLDLISTENER_H__
#define __IGAMEWORLDLISTENER_H__

#include "GameUtil.h"
#include "SmartPtr.h"

class GameWorld;
class Entity;

class IGameWorldListener
{
public:
	virtual void OnObjectAdded(GameWorld* world, Entity* object) = 0;
	virtual void OnObjectRemoved(GameWorld* world, Entity* object) = 0;
};

#endif