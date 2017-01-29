/*
A Moving Entity, which contains properties and methods to allow an entity move as well as
bounce off other objects

Author : Romesh Selvanathan
Date : 03/05/12
*/

#include "MovingEntity.h"
#include "GameUtil.h"
#include "Model_3DS.h"
#include "Graphics.h"

// Constructors
MovingEntity::MovingEntity(char const * const type_name) : Entity(type_name), mAcceleration(0, 0, 0),
	m_maxSpeed(0), m_maxForce(0), m_bIgnoreBrake(false), mVelocity(Vec3(0, 0, 0)), m_fTimeElapsed(0), m_vForce(0, 0, 0), m_fElactisity(1.0),
	m_vCollidedVelocity(Vec3(0, 0, 0)), m_fCollidedMass(1)
{
}

MovingEntity::MovingEntity(char const * const type_name, Vec3 position,Vec3 velocity,
		Vec3 acceleration,float scale) :  Entity(type_name, position,scale, 0), mAcceleration(acceleration), mVelocity(velocity),
		m_maxSpeed(0), m_maxForce(0), m_bIgnoreBrake(false), m_fTimeElapsed(0), m_vForce(0, 0, 0), m_fElactisity(1.0),
		m_vCollidedVelocity(Vec3(0, 0, 0)), m_fCollidedMass(1)
{
}

MovingEntity::MovingEntity(char const * const type_name, int& id) : Entity(type_name, id), mAcceleration(0, 0, 0), mVelocity(Vec3(0, 0, 0)),
	m_maxSpeed(0), m_maxForce(0),m_bIgnoreBrake(false), m_fTimeElapsed(0) , m_vForce(0, 0, 0), m_fElactisity(1.0),
	m_vCollidedVelocity(Vec3(0, 0, 0)), m_fCollidedMass(1)
{
}

MovingEntity::MovingEntity(char const * const type_name, int& id,Vec3 position,Vec3 velocity,
		Vec3 acceleration,float scale) :  Entity(type_name, id,position, scale, 0), mAcceleration(acceleration), mVelocity(velocity),
		m_maxSpeed(0), m_maxForce(0), m_bIgnoreBrake(false), m_fTimeElapsed(0), m_vForce(0, 0, 0), m_fElactisity(1.0),
		m_vCollidedVelocity(Vec3(0, 0, 0)), m_fCollidedMass(1)
{
}

// Descructor
MovingEntity::~MovingEntity()
{
	Entity::~Entity();
}

// This method is overloaded as the world matrix has to be updated for any objects that are using it 
// so that objects are placed in the right spot.
void MovingEntity::SetPosition(Vec3& p)
{
	mPosition = p;

	m_worldMatrix[3][0] = p.x;
	m_worldMatrix[3][1] = p.y;
	m_worldMatrix[3][2] = p.z;
}


////////////////////////////////////Protected Methods////////////////////////////////////////////////////

// Update the Orientation of the Entity(Not Local orientation, but the objects world orientation)
void MovingEntity::UpdateOrientation(float dt)
{
	// Create a temporary orientation obtaining the rotation to be applied through euler angles
	Quaternion temp;
	temp.ConvertEularToQuat(m_orientation.SetMatrix(), m_eulerOrient.x, m_eulerOrient.y, m_eulerOrient.z);
	// Apply that orientation
	m_orientation = m_orientation * temp;
	m_orientation.Normalize();

	// Multiply the current orientation with the new rotation to obtain new orientation
	temp = m_orientation * m_rotation;
	temp.Normalize();

	// Set values into world Matrix
	m_worldMatrix = temp.SetMatrix();

	// Clear the entity's cached euler rotations for this frame
    m_eulerOrient = Vec3(0.0f, 0.0f, 0.0f);
}

void MovingEntity::UpdatePosition(Vec3 force, float dt)
{
	// Obtain acceleration where F = ma so a = F/m
	mAcceleration = force / m_mass;
	// Change the z axis acceleration for that it aceelerates forward
	mAcceleration.z *=-1;
	// Set the Velocity of the Agent
	mVelocity += mAcceleration * dt;

	// Make sure velocity does not exceed max speed
	if(mVelocity.Length() > m_maxSpeed){
		mVelocity.Normalize();
		mVelocity = (mVelocity * m_maxSpeed);
	}

	// If the force is 0 and breaking should not be ignored then start braking the entity so it eventually stop
	if(!m_bIgnoreBrake && force.Length() == 0)
	{
		const float  BrakingRate = 0.98f;
		mVelocity = mVelocity * BrakingRate;
	}

	// Add position to the right, up and forward vectors change over time
    mPosition += mRight * mVelocity.x *dt;
    mPosition += mUp * mVelocity.y*dt;
    mPosition += mForward * mVelocity.z*dt;

	// Update the World Matrix
	m_worldMatrix[3][0] = mPosition.x;
    m_worldMatrix[3][1] = mPosition.y;
    m_worldMatrix[3][2] = mPosition.z;
}

void MovingEntity::ElasticCollision(Vec3 otherinitialVel)
{
		// Set the force in the opposite direction
		m_vForce *= -1;

		// The other object's mass
		float object_mass = m_fCollidedMass;
		// Obtain The intiail velocity of the other object
		Vec3 object_initVel = otherinitialVel;
		// obtain this entity's initial velocity
		Vec3 initVel = mVelocity;
		// Obtain the total sum of the two masses
		float mass_sum = m_mass + object_mass;

		Vec3 finalVel;
		// Calculate the Final velocity of this object for each axes using the formulat
		// v1 = (u1 * (m1 - m2) + 2*(m2*u2)) / (m1+m2)
		finalVel.x = (initVel.x * (m_mass - object_mass) + 2 * (object_mass * object_initVel.x)) / mass_sum;
		finalVel.y = (initVel.y * (m_mass - object_mass) + 2 * (object_mass * object_initVel.y)) / mass_sum;
		finalVel.z = (initVel.z * (m_mass - object_mass) + 2 * (object_mass * object_initVel.z)) / mass_sum;
		// Decrease the Velocity by the elactisity value to determine some energy loss when colliding - even though this is
		// a elastic collision , this creates a more unpredictable collision
		mVelocity = finalVel * m_fElactisity;
		// Increase the max speed by a bit more than what the current speed is - this will make sure that the object does not
		// speed back up to the previous max speed again, instead is capped to a new speed, depending on what the speed is
		// after the collision
		m_maxSpeed = mVelocity.Length() + 0.2f;
}