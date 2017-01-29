/*
Enemy Generator class which will generate the enemies in waves after one wave has been defeated

Author : Romesh Selvanathan
Date : 03/05/12
*/

#include "EnemyGenerator.h"
#include "ClassManager.h"
#include "GameUtil.h"
#include "SmallEnemy.h"
#include "BigEnemy.h"
#include "HardEnemy.h"
#include "GameWorld.h"
#include "PlayerManager.h"
#include "CRandom.h"
#include "Spaceship.h"
#include "GameTime.h"

const int MAX_ENEMIES = 20;				// Maximum number of small enemies that can exist in the game world at the same time
const int MAX_BIGENEMIES = 5;			// Maximum number of Big Enemies that can exist in the game world at the same time

/* Constructor */
EnemyGenerator::EnemyGenerator(SmartPtr<PlayerManager> mplayermanager, SmartPtr<Spaceship> spaceship, SmartPtr<GameTime> elapsedTime)
{
	// Player manager will be listening to each enemy to see if player has collided with enemy for update in health
	// / energy
	m_playerManager = mplayermanager;

	// Timer poiter which will be used to add each enemy as a listener of the timer
	m_pGameTime = elapsedTime;

	/* Set up each enemy */
	for(int i = 0; i < MAX_ENEMIES; i ++)
	{
		SmartPtr<SmallEnemy> enemy = new SmallEnemy(spaceship);
		m_enemies.push_back(enemy);
	}

	/* Set up each enemy */
	for(int i = 0; i < MAX_BIGENEMIES; i ++)
	{
		SmartPtr<BigEnemy> enemy = new BigEnemy(spaceship);
		m_bigEnemies.push_back(enemy);
	}

	m_pHardEnemy = new HardEnemy(spaceship);

	m_numAlive = 0;
	m_wave = 0;
	
	waveDone = false;
	timeTillNextWave = 0.0f;
	elpasedTime = 0.0f;
}
 
/* Destructor */
EnemyGenerator::~EnemyGenerator()
{
	for(int i = 0; i < m_enemies.size(); i ++)
	{
		SAFE_DELETE(m_enemies[i]);
	}

	for(int i = 0; i < m_bigEnemies.size(); i ++)
	{
		SAFE_DELETE(m_bigEnemies[i]);
	}
	
	SAFE_DELETE(m_pHardEnemy);
}

// Reset all the enemies
void EnemyGenerator:: Reset()
{
	m_numAlive = 0;
	m_wave = 0;

	waveDone = false;
	timeTillNextWave = 0.0;
	elpasedTime = 0.0f;

	for(SmallEnemyList::iterator it = m_enemies.begin(); it != m_enemies.end(); it++){
		// Add each enemy as a listener of the timer
		ClassManager::GetInstance()->RegisterClass((*it));
		m_pGameTime->AddListener((*it));
		if((*it)->IsAlive())
			(*it)->SetAlive(false);
		GameWorld::GetInstance()->AddListener((*it));
	}

	for(BigEnemyList::iterator it = m_bigEnemies.begin(); it != m_bigEnemies.end(); it++){
		// Add each enemy as a listener of the timer
		ClassManager::GetInstance()->RegisterClass((*it));
		m_pGameTime->AddListener((*it));
		if((*it)->IsAlive())
			(*it)->SetAlive(false);
		GameWorld::GetInstance()->AddListener((*it));
	}

	if(m_pHardEnemy->IsAlive())
		m_pHardEnemy->SetAlive(false);
	GameWorld::GetInstance()->AddListener(m_pHardEnemy);
	ClassManager::GetInstance()->RegisterClass(m_pHardEnemy);
	m_pGameTime->AddListener(m_pHardEnemy);
}

void EnemyGenerator::Exit()
{
	for(SmallEnemyList::iterator it = m_enemies.begin(); it != m_enemies.end(); it++){
		// Add each enemy as a listener of the timer
		ClassManager::GetInstance()->RemoveClass((*it));
	}

	for(BigEnemyList::iterator it = m_bigEnemies.begin(); it != m_bigEnemies.end(); it++){
		// Add each enemy as a listener of the timer
		ClassManager::GetInstance()->RemoveClass((*it));
	}

	ClassManager::GetInstance()->RemoveClass(m_pHardEnemy);
}

/* Adds a wave of enemies depending on the wave number currently */
void EnemyGenerator::AddWave()
{
	// A count to see how many are added and to control how many can be added
	int count = 0;
	int random = 0;

	// In this method it will iterate thorugh both the smal land big enemy list and then set them up to be 
	// Added to the game world. The count value above will basically keep track of how many are added and if 
	// the right number has been reached equal it to the number of enemies in the curren world and break the for loop
	// SO any enemy which is not alive is set to alive and then added to the game world

	if(m_wave < 3)
	{
		AddSmallEnemy(3);

		random = r.Random(101);
		if(random > 80 && random < 85)
			AddBigEnemy(1);
		if(random > 85)
			AddHardEnemy();
	}
	if(m_wave > 2 && m_wave < 10)
	{
		random = 1 + r.Random(10);
		AddSmallEnemy(random);

		random = 1 + r.Random(3);
		AddBigEnemy(random);

		random = r.Random(101);
		if(random  > 40)
			AddHardEnemy();
	}

	if(m_wave > 9)
	{
		random = 1 + r.Random(MAX_ENEMIES);
		AddSmallEnemy(random);

		random = 1 + r.Random(MAX_BIGENEMIES);
		AddBigEnemy(random);

		AddHardEnemy();
	}

	// Increment the wave
	m_wave += 1;
}

// Add a Small enemy into the world. The num parameter determines how many should be added
void EnemyGenerator::AddSmallEnemy(int num)
{
	int count = 0;

	// Find all the all the enemies currently inactive
	for(SmallEnemyList::iterator it = m_enemies.begin(); it != m_enemies.end(); it++){
			if(!(*it)->IsAlive()){
				(*it)->Reset();
				(*it)->SetAlive(true);
				Vec3 tempV = RandomPosition();
				(*it)->SetPosition(tempV);
				GameWorld::GetInstance()->AddGameObject((*it));
				GameWorld::GetInstance()->AddCollisionEntity((*it));

				// Increment count for each enemy, added, until the num value is met. At which point
				// the loop should be cancelled
				count ++;
				if(count >= num){
					m_numAlive += count;
					return;
				}
			}
	}
}

// Add a Big enemy into the world. The num parameter determines how many should be added
void EnemyGenerator::AddBigEnemy(int num)
{
	int count = 0;

	// Find all the all the enemies currently inactive
	for(BigEnemyList::iterator it = m_bigEnemies.begin(); it != m_bigEnemies.end(); it++){
			if(!(*it)->IsAlive()){
				(*it)->Reset();
				(*it)->SetAlive(true);
				Vec3 tempV = RandomBigPosition();
				(*it)->SetPosition(tempV);
				GameWorld::GetInstance()->AddGameObject((*it));
				GameWorld::GetInstance()->AddCollisionEntity((*it));

				// Increment count for each enemy, added, until the num value is met. At which point
				// the loop should be cancelled
				count ++;
				if(count >= num){
					m_numAlive += count;
					return;
				}
			}
	}
}

// Add a hard enemy if it is current Inactive.
void EnemyGenerator::AddHardEnemy()
{
	int count = 0;

	if(!m_pHardEnemy->IsAlive()){
		m_pHardEnemy->Reset();
		m_pHardEnemy->SetAlive(true);
		Vec3 tempV = RandomBigPosition();
		m_pHardEnemy->SetPosition(tempV);
		GameWorld::GetInstance()->AddGameObject(m_pHardEnemy.GetPtr());
		GameWorld::GetInstance()->AddCollisionEntity(m_pHardEnemy.GetPtr());

		count ++;
		if(count >= 1){
			m_numAlive += count;
			return;
			}
		}
}


/* Creates a random int (No use for now) */
int EnemyGenerator::RandomInt()
{
	r.Randomize();

	int i = r.Random(4);

	return i;
}

/* Generates a random position for the small Enemies */
Vec3 EnemyGenerator::RandomPosition()
{
	int i = RandomInt();

	int range = 50;
	float x = float(rand() % range) - float(range) /2.0f;
	float y = float(rand() % range) - float(range) /2.0f;
	float z = float(rand() % range) - float(range) /2.0f;

	Vec3 v = Vec3(x, y, z);

	return v;
}

/* Generates a random position for the big enemies */
Vec3 EnemyGenerator::RandomBigPosition()
{
	int i = RandomInt();

	int range = 60;
	float x = float(rand() % range) - float(range) /2.0f;
	float y = float(rand() % range) - float(range) /2.0f;
	float z = float(rand() % range) - float(range) /2.0f;

	Vec3 v = Vec3(x, y, z);

	return v;
}

/* Cheat wave */
void EnemyGenerator::CheatWave()
{
	for(SmallEnemyList::iterator it = m_enemies.begin(); it != m_enemies.end(); it++){
		if((*it)->IsAlive()){
			// Set the health to 0 so that enemy kills itself
			(*it)->SetHealth(0);
			(*it)->UpdateEnemy();
		}
	}

	for(BigEnemyList::iterator it = m_bigEnemies.begin(); it != m_bigEnemies.end(); it++){
		if((*it)->IsAlive()){
			// Set the health to 0 so that enemy kills itself
			(*it)->SetHealth(0);
			(*it)->UpdateEnemy();
		}
	}

	if(m_pHardEnemy->IsAlive()){
		// Set the health to 0 so that enemy kills itself
		m_pHardEnemy->SetHealth(0);
		m_pHardEnemy->UpdateEnemy();
	}
}

//----------------------------ITIMERLISTENER------------------------------
void EnemyGenerator::OnTimer(float milliseconds)
{
	elpasedTime = milliseconds;

	if(waveDone)
		if(elpasedTime - timeTillNextWave > 5000)
		{
			AddWave();
			waveDone = false;
		}
}
//------------------------------------------------------------------------