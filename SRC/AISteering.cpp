/*
This is essentially the brain of the Enemy. This class encapsulated all the Different Enemy behaviours
which they can use, by simply enabling or disabling them. Thi class simply returns a total force
which is used to steer an enemy some where

Author : Romesh Selvanathan
Date : 03/05/12

References : 
Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/

#include "AISteering.h"
#include "Enemy.h"
#include "MovingEntity.h"

// Constructor
AISteering::AISteering(Enemy* enemy) : m_pEnemy(enemy),
									   wander(false),
									   flee(false),
									   arrive(false),
									   seek(false),
									   evade(false)
{
	m_RandGenerator.Randomize();
	m_vWanderStartPosition = m_pEnemy->GetPosition();
	m_vWanderDestPosition = m_vWanderStartPosition;
	m_iWanderMovesCount = 0;
}

// Destructor
AISteering::~AISteering()
{
	SAFE_DELETE(m_pEnemy);
}

// Reset all the variables
void AISteering::Reset()
{
	wander = false;
	flee = false;
	arrive = false;
	seek = false;
	evade = false;
	m_RandGenerator.Randomize();
	m_vWanderStartPosition = m_pEnemy->GetPosition();
	m_vWanderDestPosition = m_vWanderStartPosition;
	m_iWanderMovesCount = 0;
}

// Calculate the Final Total Force
Vec3 AISteering::Calculate()
{
	// Reset the steering force
	m_vSteeringForce = Vec3(0, 0, 0);
	// Calculate the Force by priority
	m_vSteeringForce = CalculatePriority();

	// Make sure the force does not exceed the owner's max force
	if( m_vSteeringForce.x > m_pEnemy->GetMaxForce())
		m_vSteeringForce.x = m_pEnemy->GetMaxForce();
	if( m_vSteeringForce.y > m_pEnemy->GetMaxForce())
		m_vSteeringForce.y = m_pEnemy->GetMaxForce();
	if( m_vSteeringForce.z > m_pEnemy->GetMaxForce())
		m_vSteeringForce.z = m_pEnemy->GetMaxForce();
	// Return the total force
	return m_vSteeringForce;
}

Vec3 AISteering::CalculatePriority()
{
	// A temproary force
	Vec3 force;

	// If evade
	if(On(evade))
	{
		assert(m_pEvadeObject && "Evade target not assigned");
		// Obtain the Force to Evade
		force = Evade(m_pEvadeObject) * 150.0f;
		// Add the force to the steering and if it has exceeded the maximum then just return the final
		// force
		if(!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	// If fleeing
	if(On(flee))
	{
		force = Flee(m_pTargetObject->GetPosition()) * 50.f;
		if (!AccumulateForce(m_vSteeringForce, force))
			return m_vSteeringForce;
	}

	// If seeking
	if(On(seek))
	{
		force = Seek(m_vTarget);

		if(!AccumulateForce(m_vSteeringForce, force)) 
			return m_vSteeringForce;
	}

	// If arriving
	if(On(arrive))
	{
		m_vTarget = m_pEnemy->GetTargetObject()->GetPosition();
		force = Arrive(m_vTarget, slow);
		if(!AccumulateForce(m_vSteeringForce, force)) 
			return m_vSteeringForce;
	}

	// If wandering
	if(On(wander))
	{
		force = Wander();
		if(!AccumulateForce(m_vSteeringForce, force)) 
			return m_vSteeringForce;
	}

	// Return final force
	return m_vSteeringForce;
}

// Return the Quaternion to face the target position
Quaternion AISteering::GetQuatToFaceTarget()
{
	// The owning objects WorldMatrix to be used later
	Matrix4 ownerMatrix = m_pEnemy->GetWorldMatrix();
	// Obtain the Owner's current upVector
	Vec3 ownerUpVec = m_pEnemy->GetUpVector();
	Vec3 ownerForwardVec = m_pEnemy->GetForwardVector();

	// Obtain the direction vector to the target Object
	Vec3 direction = m_vTarget - m_pEnemy->GetPosition();
	if(direction.Length() > 0)
		direction.Normalize();
	// Atan2 gives the angle between the postitive x-axis plane and the point of (x, z(or y)) 
	// I am not using dot product + cross product to determine this as the atan2 function will automaticcly determine
	// if the turning should be positive or negative, whereas I would have to call dot product between forward and direction then use
	// cross product to see if the perpindicular axis obtained is up or down to decide whether to turn left or right. Atan2 just makes
	// things easier with one line of code
	float heading = atan2(direction.x, direction.z);
	// Obtain the Dot product between the Up and current direction and use the asin function to obtain how much to turn by
	float dot = ownerUpVec.Dot(direction);
	float pitch = -asin(dot);

	// Obtain the Up Vector and then create a quaternion from the Axis Angle put in for heading rotation (yaw)
	// create a up vector using the owner's matrix
	Vec3 up = Vec3(0, ownerMatrix[1][1], 0);
	Quaternion temp = Quaternion::IDENTITY;
	temp.ConvertAxisToQuat(heading, up);
	temp.Normalize();

	// Obtain the Right Vector and then create a quaternion from the Axis Angle put in for pitch rotation)
	// create a right vector using the owner's matrix
	Vec3 right = Vec3(ownerMatrix[0][0], 0, ownerMatrix[0][2]);
	Quaternion temp2;
	temp2.ConvertAxisToQuat(pitch, right);
	temp2.Normalize();

	// Combine the quaternions to obtain the rotation
	return (temp * temp2);
}

/////////////////////////////////////////////////PRIVATE METHODS/////////////////////////////////////////////////////////////////////////
// Accumulate all the forces together
bool AISteering::AccumulateForce(Vec3 &RunningTot, Vec3 ForceToAdd)
{
	// obtain the total force so far and the force remaining
	float magnitudeSoFar = RunningTot.Length();
	float magnitudeRemaining = m_pEnemy->GetMaxForce() - magnitudeSoFar;

	// if the remaining force is less then 0 then it has exceeded the maximum
	if(magnitudeRemaining <= 0.0f) return false;

	// obtain how much force needs to be added
	float magnitudetoAdd = ForceToAdd.Length();

	// If the force to be added is less than the force remaining then add the force
	if( magnitudetoAdd < magnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}
	// otherwise normalise the force to be added and multiply it by how much there is remaining. With
	// this value add to the total force
	else{
		ForceToAdd.Normalize();
		RunningTot += ForceToAdd * magnitudeRemaining; 
	}
	// force has been added 
	return true;
}

// Seek the target 
Vec3 AISteering::Seek(Vec3 targetPos)
{
	// Obtain the direction to travel to move to the target position
	Vec3 DesiredVel = (targetPos - m_pEnemy->GetPosition());
	DesiredVel.Normalize();

	return (DesiredVel);
}

// Flee away from target position
Vec3 AISteering::Flee(Vec3 targetPos)
{
	// Obtain the opposite direction to from the target position
	Vec3 DesiredVel = (m_pEnemy->GetPosition() - targetPos);
	DesiredVel.Normalize();

	return (DesiredVel);
}

// Arriving at the Target position
Vec3 AISteering::Arrive(Vec3 targetPos, Decelaration type)
{
	// Obtain the direction to travel to get to target position
	Vec3 toTarget = targetPos - m_pEnemy->GetPosition();
	// THe distance to the target position
	float distance = toTarget.Length();

	// If distance is greater than the arrive distance
	if(distance > m_fArriveDistance)
	{
		// This value tweaks the deceleration 
		const double decelerationTweaker = 0.3f;

		// Obtain the speed which is distance divided by the type of deceleration and the tweaker
		float speed = distance/((float) type * decelerationTweaker);

		// If the speed is greater than max speed of the enemy
		if(speed > m_pEnemy->GetMaxSpeed())
			// then the max speed should be the speed
			speed = m_pEnemy->GetMaxSpeed();

		// Obtain the force to steer the enemy
		Vec3 DesiredVel = toTarget * speed/distance;

		// return the force
		return DesiredVel;
	}
	// Otherwise return none
	return Vec3(0, 0, 0);
}

// Wandering
Vec3 AISteering::Wander()
{
	// Obtain the direction to wander to
	// (NOTE: Right at the beginning when game starts wanderDestPosition = mPosition )
	Vec3 wanderVect = m_vWanderDestPosition - m_pEnemy->GetPosition();
	// obtain the magnitude of the vector
	float wanderLength = wanderVect.Length();
	// when the position gets close to the final wander position
	if(wanderLength < EPSILON_DISTANCE)
	{
		// Generate a new Random Positon
		if(m_iWanderMovesCount < WANDER_MAX_MOVES)
		{
			m_vWanderDestPosition = m_pEnemy->GetPosition() + RandomPosition();
			m_vTarget = m_vWanderDestPosition;
			// Add the number of moves made
			m_iWanderMovesCount++;
		}
		// Or go back to the start position
		else
		{
			m_vWanderDestPosition = m_vWanderStartPosition;
			// reset the number of moves again
			m_iWanderMovesCount = 0;
		}
	}
	// otherwise still apply the current Wandering Destination as the Target, 
	// so the agent looks towards it, for example if it comes from another steering behaviour it will
	// not look towards the target without this method here unless another wander destination has been 
	// generated
	else
		m_vTarget = m_vWanderDestPosition;

	// A check to make sure that no normalization is done if magnitude is 0 or less
	if(wanderVect.Length() > 0)
		wanderVect.Normalize();

	return wanderVect;
}

// LEGACY NOT USED CURRENTLY 
Vec3 AISteering::Pursuit(const MovingEntity* object)
{
	Vec3 ToEvader = object->GetPosition() - m_pEnemy->GetPosition();

	Vec3 ownerForward = m_pEnemy->GetForwardVector();
	Vec3 evaderForward = object->GetForwardVector();
	float RelativeHeading = ownerForward.Dot(evaderForward);

	if(ToEvader.Dot(ownerForward) > 0 && (RelativeHeading < -0.95)) //acos(0.95) 18
		return Seek(object->GetPosition());

	float ownerMaxSpeed = m_pEnemy->GetMaxSpeed();
	Vec3 objectVelocity = object->GetVelocity();
	float lookAheadTime = ToEvader.Length() / (ownerMaxSpeed + objectVelocity.Length());

	return Seek(object->GetPosition() + object->GetVelocity() * lookAheadTime);
}

// Evading an object
Vec3 AISteering::Evade(const MovingEntity* object)
{
	// Obtain the vector from the position of the enemy to the object
	Vec3 toObject = object->GetPosition() - m_pEnemy->GetPosition();
	// Obtain the forward vector of the target objects
	Vec3 targetForward = m_pTargetObject->GetForwardVector();
	// obtain the Position of the target object
	Vec3 targetPosition = m_pTargetObject->GetPosition();
	// Obtain the vector from target to the enemy
	Vec3 fromTarget = m_pEnemy->GetPosition() - targetPosition;
	// Normalize the vector
	fromTarget.Normalize();
	// Obtain the Angle between the target forward vector and the vector from the object to the enemy
	float dotProduct = targetForward.Dot(fromTarget);
	float targetLookAngle = (float)acos(dotProduct);
	// If the angle is between two values then it is facing the enemy
	if(targetLookAngle*RAD2DEG > 162 && targetLookAngle*RAD2DEG < 180){
			Vec3 objectVel = object->GetVelocity();
		   // Work out the future position of the bullet by working out current distance/(enemy's max speed + the current bullet speed)
			float lookAhead = toObject.Length() /  ((m_pEnemy->GetMaxSpeed() + objectVel.Length()));
			Vec3 evadeVec = object->GetPosition() + object->GetVelocity() * lookAhead;
			Vec3 fromEvadeVec = m_pEnemy->GetPosition() - evadeVec;
			// If the object is too far then set the evade flag to off
			if(fromEvadeVec.Length() > 5)
				EvadeOff();
			// Flee by using the evade vector
			return Flee(evadeVec);
	}
	else
		EvadeOff();

	return m_vSteeringForce;
}

// LEGACY - NOT USED CURRENTLY
Vec3 AISteering::Seperate(const vector<Enemy*> &neighbours)
{
	Vec3 SteeringForce;
	
	for(int i = 0; i < neighbours.size(); i ++)
	{
		// Work out the direction to the neighbour
		Vec3 toNeighbour = m_pEnemy->GetPosition() - neighbours[i]->GetPosition();
		// Keep a copy of it
		Vec3 temp = toNeighbour;
		// Normalise the vector
		toNeighbour.Normalize();
		// scale the force inversly proportianal to the enemy's distance from its neighbour
		SteeringForce += toNeighbour/temp.Length();
	}

	return SteeringForce;
}

// This method checks if the target object is currently facing the enemy or not
bool AISteering::IsEntityFacingPlayer()
{
	Vec3 ownerForward = m_pEnemy->GetForwardVector();
	Vec3 targetPosition = m_pTargetObject->GetPosition();
	Vec3 fromTarget = m_pEnemy->GetPosition() - targetPosition;
	fromTarget.Normalize();
	float dotProduct = ownerForward.Dot(fromTarget);
	float targetLookAngle = (float)acos(dotProduct);

	test = dotProduct;

	if(targetLookAngle*RAD2DEG > 90 && targetLookAngle*RAD2DEG < 120)
		return true;
	return false;
}

// Obtain a Random Position
Vec3 AISteering::RandomPosition()
{
	// Obtain random x, y, z values which have to be a minimum of the wander distance
	float x = MIN_WANDER_DIST + static_cast<float>(m_RandGenerator.Random(15));
	float y = MIN_WANDER_DIST + static_cast<float>(m_RandGenerator.Random(15));
	float z = MIN_WANDER_DIST + static_cast<float>(m_RandGenerator.Random(15));

	// Randomly choose if x is positive or negative
	int randomInt = m_RandGenerator.Random(2);
	if(randomInt == 1)
		x *= -1;
	// Randomly choose if y is positive or negative
	randomInt = m_RandGenerator.Random(2);
	if(randomInt == 1)
		y *= -1;
	// Randomly choose if z is positive or negative
	randomInt = m_RandGenerator.Random(2);
	if(randomInt == 1)
		z *= -1;

	Vec3 random = Vec3(x, y, z);
	// return the random vector
	return random;
}