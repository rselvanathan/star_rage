/*
This is the Head of the turret. This has its own AI functions which will look at the player when in range and shoot towards him.
The Turret will have a max and minimum pitch whcih determines how much it can look up and down by. For now it does not have a proper
field of view implementation instead it compares the angle obtained from -asin to see if the player is above/below the angle as a result  not shooting.

Author : Romesh Selvanathan

Last Edit : 26/02/2012
*/

#include "BoundingSphere.h"
#include "GameWorld.h"
#include "Model_3DS.h"
#include "CRandom.h"
#include "TurretHead.h"
#include "AISteering.h"
#include "StateMachine.h"
#include "ShootManager.h"
#include "TurretHeadStates.h"

/* Constructor */
TurretHead::TurretHead(float minPitch, float maxPitch, int turretPlacement, MovingEntity* target) : Enemy( "TurretHead")
{
	mRadius = 3;
	m_hasShot = false;
	m_tillNextShot = 0;
	m_minPitchThreshold = minPitch;
	m_maxPitchThreshold = maxPitch;

	mScale = 0.2f;

	SmartPtr<Model_3DS> model = new Model_3DS();
	model->Load("3DModels//Enemy//Turrets//TurretHead3.3DS");
	SetModel(model);

	m_placement = turretPlacement;
	if(m_placement == BOTTOM)
		m_turnModifier = -1;
	else
		m_turnModifier = 1;

	m_pStateMachine = new StateMachine<TurretHead>(this);

	m_pSteering = new AISteering(this);
	m_pSteering->SetTargetObject(target);

	m_pTargetObject = target;
}

/* Reset the Turret */
void TurretHead::Reset()
{
	Enemy::Reset();

	m_hasShot = false;
	m_tillNextShot = 0;
	m_fTimeElapsed = 0;

	if(m_placement == BOTTOM)
		m_eulerOrient.z = 180.0f;

	m_pSteering->Reset();
	// Set the Current State to IdleState
	m_pStateMachine->SetCurrentState(IdleState::Instance());
	m_pStateMachine->SetPreviousState(IdleState::Instance());
	m_pStateMachine->SetGlobalState(GlobalTurretHeadState::Instance());
	// Enter that State
	m_pStateMachine->GetCurrentState()->EnterState(this);

	test = 0.0f;
}

/* Destructor */
TurretHead::~TurretHead()
{Enemy::~Enemy();}

/* Update the Object every fram */
void TurretHead::Update(float dt)
{
	// Update the State Machine for this Agent
	m_pStateMachine->Update();

	// Obtain the Orientation axis 
	m_orientation.ExtractOrientationAxis(mRight, mUp, mForward);

	// Update the Orientation of the Entity(Not Local orientation, but the objects world orientation)
	UpdateOrientation(dt);
	// Update the position of the Entity using basic physics from F=ma -> v = u + at -> S = S + vt
	UpdatePosition(Vec3(0, 0, 0), dt);

	test = m_pSteering->GetTest();
}

bool TurretHead::HandleMessage(const Telegram& msg)
{
	if(m_pStateMachine->HandleMessage(msg))
		return true;
	return false;
}

// Attack the Player without moving but turning - This method is specfic to the turret so keeping it here
void TurretHead::FaceTarget()
{
	// The Direction to face
	Vec3 direction = m_pTargetObject->GetPosition() - mPosition;
	// A safety check to make sure no normalizations occur when length is 0
	if(direction.Length() > 0)
		direction.Normalize();

	// Turret looks up slightly so made the direction be down slighty (trial and error)
	direction.y -= 0.1f;

	// Should the turret shoot?
	bool shoot = true;				

	// Obtain the yaw degrees from the player
	// Atan2 gives the angle between the postitive x-axis plane and the point of (x, z(or y)) 
	// I am not using dot product + corss product to determine this as the atan2 function will automaticcly determine
	// if the turning should be positive or negative, whereas I would have to call dot product between forward and direction then use
	// cross product to see if the perpindicular axis opbtained is up or down to decide whether to turn left or right. Atan2 just makes
	// things easier with one line of code (may be less efficient have to check, but it creates a smoother transition)
	float heading = atan2(direction.x, direction.z);
	// Obtain the Dot product between the Up and current direction and use the asin function to obtain how much to turn by. As the direction will be lower
	// then the up vector the turret has to generally move down to equal the up vector to the current direction so i am turning the angle negative.
	float dot = mUp.Dot(direction);
	float pitch = -asin(dot);
	// These will limit the rotation in pitch by cetain amount and dissallow shooting if it is beyond the threshold
	if((pitch * RAD2DEG) < m_minPitchThreshold){
		pitch = m_minPitchThreshold * DEG2RAD;
		shoot = false;
	}
	if((pitch * RAD2DEG) > m_maxPitchThreshold){
		pitch = m_maxPitchThreshold * DEG2RAD;
		shoot = false;
	}

	// Obtain the Up Vector and then create a quaternion from the Axis Angle put in for heading rotation (yaw)
	Vec3 up = Vec3(0, m_worldMatrix[1][1], 0);
	Quaternion temp = Quaternion::IDENTITY;
	temp.ConvertAxisToQuat(heading * m_turnModifier, up);
	temp.Normalize();

	// Obtain the Right Vector and then create a quaternion from the Axis Angle put in for pitch rotation)
	Vec3 right = Vec3(m_worldMatrix[0][0], 0, m_worldMatrix[0][2]);
	Quaternion temp2;
	temp2.ConvertAxisToQuat(pitch, right);
	temp2.Normalize();

	// Combine the quaternions to obtain the rotation
	m_rotation = temp * temp2 ;

	// if shoot is enabled and there is no delay shoot towards the target
	if(shoot)
		if(!m_bIgnoreShot)
			ShootBullet(direction);
}

/* Shoot A bullet */ 
void TurretHead::ShootBullet(Vec3 direction)
{
	// If the shot did not complete recently
	if(!m_hasShot){
		// Preset an acceleration
		float bullet_acceleration = 20;
		// Determine velocity
		Vec3 velocity =  direction * bullet_acceleration;
		//Left Shot
		// increate the x/y position to position the bullet properly
		// These were trial and error values
		Vec3 shootPosition = mPosition;
		shootPosition.y += 0.45f * m_turnModifier;
		shootPosition.x -= 0.4f * m_turnModifier;
		// Create new Shoot Object with enemy position and the velocity worked out
		ShootManager::GetInstance()->AddShot("enemybullet", shootPosition, velocity, 200, Vec3(0.8, 0.0, 0.0), m_worldMatrix, 0.5f); 
		// Right Shot
		Vec3 shootPos = mPosition;
		shootPos.y += 0.45f * m_turnModifier;
		shootPos.x += 0.48f * m_turnModifier;
		// Create new Shoot Object with enemy position and the velocity worked out
		ShootManager::GetInstance()->AddShot("enemybullet", shootPos, velocity, 200, Vec3(0.8, 0.0, 0.0), m_worldMatrix, 0.5f); 
		// enemy has shot recently
		m_hasShot = true;

		m_tillNextShot = m_fTimeElapsed;
	}
}


//------------------------------ITimerListener--------------------------------
// Timer to decrease any time values
void TurretHead::OnTimer(float milliseconds)
{
	m_fTimeElapsed = milliseconds;

	if(m_hasShot) 
		if(milliseconds - m_tillNextShot >=2000)
			m_hasShot = false;

	if(m_bIgnoreShot)
		if(m_fTimeElapsed - m_fTimeIgnoreShoot > 100)
			m_bIgnoreShot = false;
}

//------------------------------------------------------------------------------