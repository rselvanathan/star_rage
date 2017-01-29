/*
This class Generates powerups into the world depending on the current time since game was started. It keeps a list of precreated powerups
and adds them into the world so "new" is not called during run time which is a expensive call to make eating up cpu time. Instead I am sacrificing
memory but due to  the game being small I can take this route.

Author : Romesh Selvanathan

Last Edit : 01/03/2012
*/

#include "GameUtil.h"
#include "GameWorld.h"
#include "PowerUp.h"
#include "PowerUpManager.h"
#include <algorithm>

// Const values 
const int MAX_HEALTH_POWERUPS = 5;
const int MAX_ENERGY_POWERUPS = 5;
const int MAX_POWERUPS_INWORLD = 3;
const int TIME_TILL_NEXT_ADD = 10000;
const int TIME_TILL_NEXT_CONT_ADD = 20000;

PowerUpManager* PowerUpManager::mPowerUpManager;

/* Constructor that accepts the current game world as a pointer to be used to add the power ups */
PowerUpManager::PowerUpManager()
{
	// Create the PowerUps at the beginning and add them to their respective container
	for(int i = 0; i < MAX_HEALTH_POWERUPS; i ++) {
		SmartPtr<PowerUp> p = new PowerUp("poweruphealth");
		m_healthpowers.push_back(p);
	}
	for(int i = 0; i < MAX_ENERGY_POWERUPS; i++) {
		SmartPtr<PowerUp> p = new PowerUp("powerupenergy");
		m_energypowers.push_back(p);
	}
}
/* Destructor */
PowerUpManager::~PowerUpManager()
{
	if(!m_healthpowers.empty())
		for(PowerUpList::iterator it = m_healthpowers.begin(); it != m_healthpowers.end(); it++)
			SAFE_DELETE(*it);
	if(!m_energypowers.empty())
		for(PowerUpList::iterator it = m_energypowers.begin(); it != m_energypowers.end(); it++)
			SAFE_DELETE(*it);
}
/* Initialize the lists of powerups and values */
bool PowerUpManager::Init()
{
	// if the list is not empty then look for any powerups that are set to being "alive" and set them as being "not alive"
	if(!m_healthpowers.empty()) {
		for(PowerUpList::iterator it = m_healthpowers.begin(); it != m_healthpowers.end(); it++)
			if((*it)->IsAlive())
				(*it)->SetAlive(false);
	}

	if(!m_energypowers.empty()) {
		for(PowerUpList::iterator it = m_energypowers.begin(); it != m_energypowers.end(); it++)
			if((*it)->IsAlive())
				(*it)->SetAlive(false);
	}
	// Set the seed for the random Generator
	m_random.Randomize();
	// Init the variables
	m_timeSinceLastUpdate = 0;
	m_timeElapsed = 0;
	m_objectRemoved = true;
	m_TimeSinceObjectAdd = 0;
	m_iObjectsToAdd = MAX_POWERUPS_INWORLD;
	// Everything went ok
	return true;
}
/* Add a new PowerUp into the world */
void PowerUpManager::AddPowerUp()
{
	// Obtain a random number which determine which power up to be added
	int randomNum = m_random.Random(2);

	if(randomNum == 0)
		// If List is not empty find a Powerup that is "not alive" and set it as alive and add it to the game world
		if(!m_healthpowers.empty()) 
			for(PowerUpList::iterator it = m_healthpowers.begin(); it != m_healthpowers.end(); it++)
				if(!(*it)->IsAlive()){
					(*it)->SetAlive(true);
					(*it)->SetPosition(GenerateRandomPosition());
					//(*it)->SetPosition(Vec3(20, 0, -30));
					GameWorld::GetInstance()->AddGameObject((*it).GetPtr());
					GameWorld::GetInstance()->AddCollisionEntity((*it).GetPtr());
					m_activePowerUps.push_back((*it));
					// count down the objects to add
					m_iObjectsToAdd -= 1;
					Clamp(m_iObjectsToAdd, 0, MAX_POWERUPS_INWORLD);
					return;
				}

	if(randomNum == 1)
		// If List is not empty find a Powerup that is "not alive" and set it as alive and add it to the game world
		if(!m_energypowers.empty()) 
			for(PowerUpList::iterator it = m_energypowers.begin(); it != m_energypowers.end(); it++)
				if(!(*it)->IsAlive()){
					(*it)->SetAlive(true);
					(*it)->SetPosition(GenerateRandomPosition());
					//(*it)->SetPosition(Vec3(10, 0, -30));
					GameWorld::GetInstance()->AddGameObject((*it).GetPtr());
					GameWorld::GetInstance()->AddCollisionEntity((*it).GetPtr());
					m_activePowerUps.push_back((*it));
					// count down the objects to add
					m_iObjectsToAdd -= 1;
					Clamp(m_iObjectsToAdd, 0, MAX_POWERUPS_INWORLD);
					return;
				}			
}

/* Remove a powerup by setting it as "not alive" */
void PowerUpManager::RemovePowerUp(Entity* object)
{
	// If the object removed is a power up then find it in the respective list
	if(object->GetType() == GameObjectType("poweruphealth")){
		PowerUpList::iterator it = find(m_healthpowers.begin(), m_healthpowers.end() - 1, object);
		// this makes sure that the found object is not the last item in the container - if "find" does not find the value given 
		// it will equal the last value to "it" 
		// Reference - http://www.cplusplus.com/reference/algorithm/find/
		if(*it != object)
			return;
		else {
			(*it)->SetAlive(false);
			// remove it from the currently Active powerups
			m_activePowerUps.erase(remove(m_activePowerUps.begin(), m_activePowerUps.end(), (*it)));
		}
	}
	// If the object removed is a power up then find it in the respective list
	if(object->GetType() == GameObjectType("powerupenergy")){
		PowerUpList::iterator it = find(m_energypowers.begin(), m_energypowers.end() - 1, object);
		// this makes sure that the found object is not the last item in the container - if "find" does not find the value given 
		// it will equal the last value to "it" 
		// Reference - http://www.cplusplus.com/reference/algorithm/find/
		if(*it != object)
			return;
		else {
			(*it)->SetAlive(false);
			// remove it from the currently Active powerups
			m_activePowerUps.erase(remove(m_activePowerUps.begin(), m_activePowerUps.end(), (*it)));
		}
	}
}

/* Generate a Random Positio for the powerups to be placed in */
Vec3 PowerUpManager::GenerateRandomPosition()
{
	// Temp to be returned in the end
	Vec3 temp;
	// Obtain random x, y, z values 
	float x = 1 + static_cast<float>(m_random.Random(50));
	float y = 1 + static_cast<float>(m_random.Random(50));
	float z = 1 + static_cast<float>(m_random.Random(50));

	// Randomly choose if x is positive or negative
	int randomsign = m_random.Random(2);
	if(randomsign == 1)
		x *= -1;
	// Randomly choose if y is positive or negative
	randomsign = m_random.Random(2);
	if(randomsign == 1)
		x *= -1;
	// Randomly choose if z is positive or negative
	randomsign = m_random.Random(2);
	if(randomsign == 1)
		z *= -1;

	temp = Vec3(x, y, z);
	// return the random vector
	return temp;
}

////////////////////////////////////IGameWorldListener///////////////////////////////////////
void PowerUpManager::OnObjectRemoved(GameWorld* world, Entity* object)
{
	// if Object is a power up then remove it
	if(object->GetType() == GameObjectType("poweruphealth") || object->GetType() == GameObjectType("powerupenergy")){
		RemovePowerUp(object);
		// set the flag to true which will add a new powerup after a certain time limit
		m_objectRemoved = true;
		// obtain the current elapsed time 
		m_timeSinceLastUpdate = m_timeElapsed;
		m_iObjectsToAdd += 1;
		Clamp(m_iObjectsToAdd, 0, MAX_POWERUPS_INWORLD);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////ITimerListener///////////////////////////////////////////
void PowerUpManager::OnTimer(float milliseconds)
{
	// Update the time elpased
	m_timeElapsed = milliseconds;

	// if a powerup has been removed recently
	if(m_objectRemoved)
		// if the difference between the last time a powerup was removed and current time is >= then a specified add
		// a new powerup and unflag the boolean
		if(milliseconds - m_timeSinceLastUpdate >= TIME_TILL_NEXT_ADD){
			// Add Objects depending on how many there are
			// objects to add + 1 to make sure more than the amount are added
			for(int i = 0; i < m_iObjectsToAdd + 1; i++)
				AddPowerUp();
			m_objectRemoved = false;
		}
}
/////////////////////////////////////////////////////////////////////////////////////////////