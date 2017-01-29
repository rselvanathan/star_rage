/*
This class Generates powerups into the world depending on the current time since game was started. It keeps a list of precreated powerups
and adds them into the world so "new" is not called during run time which is a expensive call to make eating up cpu time. Instead I am sacrificing
memory but due to  the game being small I can take this route.

Author : Romesh Selvanathan

Last Edit : 01/03/2012
*/

#ifndef __POWERUPMANAGER_H__
#define __POWERUPMANAGER_H__

#include "IGameWorldListener.h"
#include "ITimerListener.h"
#include <vector>
#include "CRandom.h"
#include "SmartPtr.h"

class PowerUp;
class Entity;

class PowerUpManager : public IGameWorldListener, public ITimerListener
{
private:
	typedef vector<PowerUp*> ActivePowerUps;
	typedef vector<SmartPtr<PowerUp>> PowerUpList;

public:
	static PowerUpManager* GetInstance()
	{
		if(mPowerUpManager == NULL)
			mPowerUpManager = new PowerUpManager();
		return mPowerUpManager;
	} 
	/* Destructor */
	~PowerUpManager();
	/* Initialize the lists of powerups and values */
	bool Init();

	////////////////////////////////////IGameWorldListener///////////////////////////////////////
	void OnObjectAdded(GameWorld* world, Entity* object){}
	void OnObjectRemoved(GameWorld* world, Entity* object);
	/////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////ITimerListener///////////////////////////////////////////
	 void OnTimer(float milliseconds);
	/////////////////////////////////////////////////////////////////////////////////////////////

	 ActivePowerUps GetActivePowerUps() const {return m_activePowerUps;}

private:
	static PowerUpManager* mPowerUpManager;
	/* Constructor  */
	PowerUpManager();

	ActivePowerUps m_activePowerUps;				// A list of all the active powerups currently
	PowerUpList m_energypowers;						// List of energy power ups
	PowerUpList m_healthpowers;						// List of health power ups

	void AddPowerUp();								// Add a new PowerUp into the world
	void RemovePowerUp(Entity* object);			// Remove a powerup by setting it as "not alive"
	Vec3 GenerateRandomPosition();					// Generate a Random Positio for the powerups to be placed in

	CRandom m_random;								// Random Number Generator

	int m_timeSinceLastUpdate;						// Time Since the last update occured in seconds
	int m_timeElapsed;								// The current time since the game session started
	bool m_objectRemoved;							// flag for if a powerup has been removed
	int m_iObjectsToAdd;							// How many objects should be added constantly
	int m_TimeSinceObjectAdd;						// Time since last time and object was added - used for continued object addition, to give a time lapse between each addition
};

#endif