/*
Enemy Generator class which will generate the enemies in waves after one wave has been defeated

Author : Romesh Selvanathan
Date : 03/05/12
*/

#ifndef __ENEMYGENERATOR_H__
#define __ENEMYGENERATOR_H__

#include "IGameWorldListener.h"
#include "GameUtil.h"
#include "CRandom.h"
#include "MovingEntity.h"
#include "ITimerListener.h"

class SmallEnemy;
class BigEnemy;
class HardEnemy;
class Enemy;
class PlayerManager;
class Spaceship;
class GameTime;

class EnemyGenerator : public IGameWorldListener, public ITimerListener
{
public:
	/* Constructor */
	EnemyGenerator(SmartPtr<PlayerManager> mplayermanager, SmartPtr<Spaceship> spaceship, SmartPtr<GameTime> elapsedTime);
	/* Destructor */
	~EnemyGenerator();

	void Reset();
	/* This method will be called when the gamplay screen exits */
	void Exit();

	/* Cheat wave */
	void CheatWave();

	/* Adds a wave of enemies depending on the wave number currently */
	void AddWave();
	// Add a Small enemy into the world. The num parameter determines how many should be added
	void AddSmallEnemy(int num);
	// Add a Big enemy into the world. The num parameter determines how many should be added
	void AddBigEnemy(int num);
	// Add a hard enemy if it is current Inactive.
	void AddHardEnemy();

	/* Creates a random int (No use for now) */
	int RandomInt();
	/* Generates a random position for the small Enemies and Hard Enemies */
	Vec3 RandomPosition();
	/* Generates a random position for the big enemies */
	Vec3 RandomBigPosition();

	//////////////////////////////////////////////////IGAMEWORLDLISTENER////////////////////////////////////////////////////
	void OnObjectAdded(GameWorld* world, Entity* object){}
	void OnObjectRemoved(GameWorld* world, Entity* object)
	{
		// If removed object was one of the enemies set them as not alive decrease the number of enemies in the world by 1
		// and then if it reaches 0 Add the new wave
		if( object->GetType() == GameObjectType("smallenemy") || object->GetType() == GameObjectType("bigEnemy")
			|| object->GetType() == GameObjectType("hardenemy")){
			object->SetAlive(false);
			m_numAlive -= 1;
			if(m_numAlive <= 1 ){
				if(m_numAlive < 0)
					m_numAlive = 0;
				waveDone = true;
				timeTillNextWave = elpasedTime;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------ITIMERLISTENER------------------------------
	void OnTimer(float milliseconds);
	//------------------------------------------------------------------------

private:
	int m_numAlive;												// Current number of Enemies alive in the world
	int m_wave;													// The wave number
			
	SmartPtr<PlayerManager> m_playerManager;					// Pointer to the Player Manager
	SmartPtr<GameTime> m_pGameTime;								// Pointer to a GameTime class

	typedef vector<SmartPtr<SmallEnemy>> SmallEnemyList;
	SmallEnemyList m_enemies;									// List of Small enemies

	typedef vector<SmartPtr<BigEnemy>> BigEnemyList;
	BigEnemyList m_bigEnemies;									// List of Big enemies

	SmartPtr<HardEnemy> m_pHardEnemy;							// Only one hard Enemy

	CRandom r;													// Random Number Generator

	bool waveDone;												// Has the wave been completed?
	float timeTillNextWave;										// Time till the next wave is added
	float elpasedTime;											// Current elapsed time

	enum {SMALL, HARD, BIG};
};

#endif