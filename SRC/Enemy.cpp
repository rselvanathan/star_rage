/*
This is the base class for all the enemies. This class constains some important properties
for any type of enemy, such as health and energy. It also contains the pointers to the AISteering
and the Targeting class.
The class finally provides the different utlity functions that determine what an object prefers,
depending on some properties.

Author : Romesh Selvanathan
Date : 03/05/12
*/

#include "Enemy.h"
#include "GameUtil.h"
#include "MovingEntity.h"
#include "Model_3DS.h"
#include "GameWorld.h"
#include "BoundingSphere.h"
#include "AISteering.h"

Enemy::Enemy(const char* typeName) : MovingEntity(typeName),
									 mHealth(1),
									 speedMultiplier(1),
									 m_fEnergy(MAX_ENERGY),
									  m_bIgnoreShot(true),
									m_fTimeIgnoreShoot(0)
{
	random.Randomize();
}

Enemy::Enemy(const char* typeName , int& id) : MovingEntity(typeName, id),
												mHealth(1),
												speedMultiplier(1),
												m_fEnergy(MAX_ENERGY),
												 m_bIgnoreShot(true),
												m_fTimeIgnoreShoot(0)
{
	random.Randomize();
}

Enemy::~Enemy()
{
	SAFE_DELETE(m_pTargetObject);
	SAFE_DELETE(m_pSteering);
	if(!m_potentialTargets.empty()){
		for(PotentialTargetList::iterator it = m_potentialTargets.begin(); it != m_potentialTargets.end(); it++)
			SAFE_DELETE(*it);
	}
	MovingEntity::~MovingEntity();
}

void Enemy::Reset()
{
	m_bIgnoreShot = true;
	m_fTimeIgnoreShoot = 0;

	m_worldMatrix.Identity();
	m_orientation.Identity();
	m_rotation.Identity();
    
    mRight = Vec3(1.0f, 0.0f, 0.0f);
    mUp = Vec3(0.0f, 1.0f, 0.0f);
    mForward = Vec3(0.0f, 0.0f, -1.0f);
	mRed = mGreen = mBlue = 1.0f;
    
    m_eulerOrient = Vec3(0.0f, 0.0f, 0.0f);

	random.Randomize();

	GeneratePersonality();
}

void Enemy::Render()
{
	Graphics::GetInstance()->MultiplyToMatrix(m_worldMatrix);

	mModel->rot.x = pitch;
	mModel->rot.y = yaw;
	mModel->rot.z = roll;
	mModel->color.x = mRed;
	mModel->color.y = mGreen;
	mModel->color.z = mBlue;
	mModel->scale = mScale;
	mModel->Draw();	
}

// References From Programming Game AI by example - Chapter 9
float Enemy::CalcHealthDesirability(float distance, float maxDistance)
{
	// If distance is greater than maximum disregard it
	if(distance > maxDistance)
		return 0.0f;

	const float tweaker = 0.5f;

	// Desirability is directly proportional to how much health is left. its 1 - health/MAXhealth, because
	// 1 - the health left will give the opposite fraction of how much there is. So the lower the health the
	// closer to the 1 the value is making it more desirable. 
	// Then / percentage of distance as, Desirability is indirectly proportional to the distance, so the higher the distance
	// the lower the value meaning the total fraction will be less. THe distance is also squared, to make sure that the
	// distance does not create a linear graph . Instead I was the agent to choose the target at
	// a much higher rate if it is really close. By incresing the value by a exponential amount I am creating a curved graph 
	// which will be more non-linear, allowing the agent to be more decisive.
	// Tweaker as the name suggests tweaks the value
	float Desir = tweaker*(1.0f - (mHealth/3.0f)) / (pow(distance/maxDistance, 2));
	
	// Clamp the value
	Clamp(Desir, 0, 1);

	// Changes the value depending on the personality
	Desir *= m_fHealthBias;

	return Desir;
}

// References From Programming Game AI by example - Chapter 9
float Enemy::CalcPlayerDesirability(float distance, float maxDistance)
{
	const float tweaker = 1.0f;

	// a value between 0 and 1
	// working out energy fraction straight * health fraction straight as these two will determine how much the
	// enemy seeks the player. If energy is low then the final value is low and if health is low the final value is low aswel
	// Distance again is indrectly proportional so the higgher the distance the lower it will seek the player
	double Desir = tweaker* (m_fEnergy/MAX_ENERGY) * (mHealth/3.0f) / (pow(distance/maxDistance, 2));

	Clamp(Desir, 0, 1);

	// Change the value depending on the personality
	Desir *= m_fPlayerBias;

	return Desir;
}

// References From Programming Game AI by example - Chapter 9
float Enemy::CalcEnergyDesirability(float distance, float maxDistance)
{
	// If distance is greater than maximum disregard it
	if(distance > maxDistance)
		return 0.0f;

	const float tweaker = 0.15f;
	const float bias = 0.2f;
	
	// Again very similar to the CalHealthdesir, but this time health is also included in the calculation.
	// This is because the lower the health the lower this final value should be, as the enemey's first
	// priority should be health not energy
	float Desir = (tweaker* (mHealth/3.0f) * (1-(m_fEnergy/MAX_ENERGY))) /(pow(distance/maxDistance, 2));

	Clamp(Desir, 0, 1);

	Desir *= m_fEnergyBias;

	return Desir;
}

void Enemy::GeneratePersonality()
{
	// The minimum Player Bias should be high
	// as I want the agent to take the player into higher
	// consideration at a higher chance
	float minPlayerBias = 70.0f;

	// Max and Minimum health bias
	float minHealthBias = 20.0f;
	float maxHealthBias = 90.0f;

	// Max and min of energy bias
	float minEnergyBias = 15.0f;
	float maxEnergyBias = 80.0f;

	// Generate Player bias, with a minimum bias
	m_fPlayerBias = static_cast<float>(random.Random(101));
	Clamp(m_fPlayerBias, minPlayerBias, 100);

	// Generate Health bias, and Clamp the values
	m_fHealthBias = static_cast<float>(random.Random(101));
	Clamp(m_fHealthBias, minHealthBias, maxHealthBias);

	// Generate Energy bias, and Clamp the values
	m_fEnergyBias = static_cast<float>(random.Random(101));
	Clamp(m_fEnergyBias, minEnergyBias, maxEnergyBias);

	// Turn the bias values into a fraction/decimal betwen 0 and 1
	m_fPlayerBias /=100.0f;
	m_fHealthBias /=100.0f;
	m_fEnergyBias /=100.0f;
}