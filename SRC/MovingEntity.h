/*
A Moving Entity, which contains properties and methods to allow an entity move as well as
bounce off other objects

Author : Romesh Selvanathan
Date : 03/05/12
*/

#ifndef __MOVINGENTITY_H__
#define __MOVINGENTITY_H__

#include "GameUtil.h"
#include "GameObjectType.h"
#include "Entity.h"
#include "ITimerListener.h"

class Telegram;

class MovingEntity : public Entity, public ITimerListener
{
public :
	// Constructors
	MovingEntity(char const * const type_name);
	MovingEntity(char const * const type_name, int& id);
	MovingEntity(char const * const type_name,Vec3 position,Vec3 velocity,
		Vec3 acceleration,float scale);
	MovingEntity(char const * const type_name, int& id,Vec3 position,Vec3 velocity,
		Vec3 acceleration,float scale);
	// Descructor
	virtual ~MovingEntity();

	// Methods Inherited From Entity
	virtual bool Init() = 0;
	virtual bool CleanUp(){return false;}
	void Reset();
	virtual void Update(float dt) = 0;
	virtual bool HandleMessage(const Telegram& msg){return true;} 

	// Set Methods
	// Overload SetPosition as some objects may use this differently
	virtual void SetPosition(Vec3& p);
	void SetForce(const Vec3& f) {m_vForce = f;}
	void SetVelocity(const Vec3& v) { mVelocity = v; }
	void SetAcceleration(const Vec3& a) { mAcceleration = a; }
	void SetMaxSpeed(float s) {m_maxSpeed = s;}
	void SetElactisity(float e) {m_fElactisity = e;}

	// Get Methods
	const float GetElapsedTime() const {return m_fTimeElapsed;}
	const Vec3 GetForce() const {return m_vForce;}
	const Vec3 GetVelocity() const { return mVelocity; }
	const Vec3 GetAcceleration() const { return mAcceleration; }
	const float GetMaxSpeed() const {return m_maxSpeed;}
	const float GetMaxForce() const {return m_maxForce;}
	const float GetElactisity() const {return m_fElactisity;}

	//-------------------------------ITimerListener-------------------------------------
	virtual void OnTimer(float milliseconds) = 0;
	//----------------------------------------------------------------------------------

protected :
	Vec3 mVelocity;				// Current Velocity of object
	Vec3 mAcceleration;			// Current Acceleration of object
	Vec3 m_vForce;				// Current Force exerted on the object;
	float m_maxSpeed;			// Maximum speed this entity may travel at				
	float m_maxForce;			// The maximum force this entity may produce (as in rockets or thrust)
	// how elastic an object is - used for collision, higher the elasticity the more bouncy it becomes (between 0.0 and 1.0)
	float m_fElactisity;		
	bool m_bIgnoreBrake;		// Flag to check whther the velocity should decrease during force = 0
	float m_fTimeElapsed;		// Time Elapsed since game started

	// These values hold the velocity and mass when elastic collision occurs of this entity. The reason for this is due to the way
	// the game is updated. If a object is in front of another object in the update list, then if the other object, updates it velocity using
	// this entity's initial velocity, the other object would have obtained the final velocity. So when it gets around to updating this entity
	// instead of obtaining the initial velocity at the collision, your now obtaining the updated velocity. This in turn will cancel out the elastic
	// collision. So these temporary place holders, will contain the initial velocity and final velocity at the point of collision, which then can be
	// sent as a event message to the other object at a delayed time, to make sure th eupdated velocity is not sent
	Vec3 m_vCollidedVelocity;	// Velocity at the point of collision
	// This is here as the The message event system cannot send more than one additional data. So this will allow mass of the other object to be held when collision
	// message is sent, which in turn can be used in the calculation
	float m_fCollidedMass;		// Mass at the point of collision of the other object;

	// Update the Orientation of the Entity(Not Local orientation, but the objects world orientation)
	void UpdateOrientation(float dt);
	// Update the position of the Entity using basic physics from F=ma -> v = u + at -> S = S + vt
	void UpdatePosition(Vec3 force, float dt);
	// Cause an elastic collision where total momentum before = total momentum after
	void ElasticCollision(Vec3 otherinitialVel);
};

#endif