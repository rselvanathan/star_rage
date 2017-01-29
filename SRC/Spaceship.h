/*
The Player spaceship

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include "MovingEntity.h"
#include "GameUtil.h"
#include "IPlayerListener.h"

class EngineEnergyBall;
class Shoot;
class Camera;

class Spaceship : public MovingEntity, public IPlayerListener
{
public:
	Spaceship();
	virtual ~Spaceship();

	bool Init(){return true;}
	void Reset();

	virtual void Update(float dt);
	virtual void Render();

	virtual bool CollisionTest(Entity* o);
	virtual void OnCollision(Entity* o);

	virtual bool HandleMessage(const Telegram& msg);

	///////////////////////////////////////////////////IPlayerListener///////////////////////////////////////////
	void OnHealthChanged(int health){}
	void OnEnergyChanged(float energy);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//-------------------------------ITimerListener-------------------------------------
	void OnTimer(float milliseconds);
	//----------------------------------------------------------------------------------

	//-------------------------------Player Control Methods-------------------------------------
	void MoveForward(Vec3 force);
	void MoveBackwards(Vec3 force);
	void ShootBullet();
	//------------------------------------------------------------------------------------------

	SmartPtr<Camera> GetCamera() const {return m_pCamera;}

	// Change the Direction the object is facing
	void SetPitch(float v);
	void SetHeading(float v);
	void SetRoll(float v);
	void orient(float headingDegrees, float pitchDegrees, float rollDegrees); //- LEGACY used before until above 3

	void SetSprint(bool s) {m_sprint = s;}

private:
	Vec3 heading;									// The current heading vector
    Vec3 eulerOrientElapsed;						// the three rotations per second

	float m_timeTillNextShot;						// A delay for shooting
	bool hasShot;

	float m_timeTillEnemyShoot;						// A delay for testing collision with enemy shots
	bool enemyShot;
				
	bool energyDepleted;							// A check to see if energy has depleted if yes cant shoot anymore
	bool m_sprint;

	float m_fdeltaTime;								// Elapsed time since game began

	SmartPtr<Camera> m_pCamera;						// COntains a pointer to the camera. THis class will update the camera and control it.
	SmartPtr<EngineEnergyBall> m_pEngineEnergy;		// Contains a energy engine, that will be updated based on the velocity of the player
};

#endif