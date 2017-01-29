/*
A Base Class for all Game Objects that are static

Author : Romesh Selvanathan
*/
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Telegram.h"
#include "GameUtil.h"
#include "GameObjectType.h"
#include "BaseClass.h"

class GameWorld;
class BoundingSphere;
class BoundingBox;
class Model_3DS;
class GameObject;

class Entity : public BaseClass 
{
public:
	// Destructor
	virtual ~Entity()
	{
		// Remove Pointer to the owning world
		SAFE_DELETE(mWorld);
	}

	// All Entities must implement these functions
	virtual void Update(float dt)=0;
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	// All entities can communicate using messages. They are send using the MessageDispatcher class
	virtual bool HandleMessage(const Telegram& msg) = 0;

	// Entites may collide with Other objects
	virtual bool CollisionTest(Entity* o) { return false; }
	virtual bool CollisionPlaneTest(Entity* o) {return false;}
	virtual void OnCollision(Entity* o) {}

	// Accessor Methods
	const GameObjectType& GetType() const { return mType;}
	Vec3 GetPosition() const {return mPosition;}
	const float GetScale() const {return mScale;}
	const float GetRadius() const {return mRadius;}
	const float GetMass() const {return m_mass;}
	const bool IsAlive() const {return mAlive;}
	const Matrix4 &GetWorldMatrix() const{ return m_worldMatrix;}
	const Vec3 &GetForwardVector() const{return mForward;}
	const Vec3 &GetRightVector() const{return mRight;}
	const Vec3 &GetUpVector() const{return mUp;}
	const Vec3 &GetOrientation() const {return m_eulerOrient;}
	const float GetPitch() const {return pitch;}
	const float GetYaw() const {return yaw;}
	const float GetRoll() const {return roll;}
	const SmartPtr<BoundingSphere> GetBoundingShape() const { return mBoundingSphere; }
	const SmartPtr<BoundingBox> GetBoundingBox() const { return mBoundingBox;}
	const GameWorld* GetWorld(){return mWorld;}
	// Mutator Methods
	// This is virtual as some object may be shared by same type but may have different outcomes when this is set (eg bullets)
	virtual void SetGameObjectType(char const* const type) {mType.SetTypeName(type);}
	virtual void SetPosition(Vec3& pos){mPosition = pos;}
	void SetScale(float s) {mScale = s;}
	void SetRadius (float r) {mRadius = r;}
	void SetMass(float m) {m_mass = m;}
	void SetAlive(bool b) {mAlive = b;}
	void SetYaw(float y){yaw += y;}
	void SetPitch(float p){pitch += p;}
	void SetRoll(float r){roll += r;}
	void SetColor(float r, float g, float b) {mRed = r; mGreen = g; mBlue = b;}
	void SetWorldMatrix(const Matrix4 &worldMatrix);
	void SetGameWorld(GameWorld* world){mWorld = world;}
	void SetModel(SmartPtr<Model_3DS> model) {mModel = model;}
	
protected:
	// Position of Entity
	Vec3 mPosition;			
	// The Type of Game entity this is (e.g player, enemy, bullets)
	GameObjectType mType;
	// Scale of Entity
	float mScale;
	// The boundingRadius of Entity
	float mRadius;
	// Mass of Entity
	float m_mass;		
	// A tag
	bool mAlive;
	// A tag that ignores collision detection after it has occured - this makes sure that if a
	// collision has occured recently to ignore it for a specfied time
	bool m_bIgnoreCollision;
	// The World matrix for the entity
	Matrix4 m_worldMatrix;	
	// The current orientation for the entity (this is relative to the world)
	Quaternion m_orientation;
	// The rotation to be applied to rotate
	Quaternion m_rotation;
	// The forward vector of the entity
	Vec3 mForward;	
	// The up Vector of the entity
	Vec3 mUp;	
	// The Right Vector of the entity
	Vec3 mRight;	
	// The Euler orientation for the entity(another way of changing the orientation rather than using a Quaternion - in degrees)
	Vec3 m_eulerOrient;	
	// maximum turn rate(radians per second) 
	float m_maxTurnRate;
	// The LOCAL orientation values , for ex you can have a ball rotationg around its own LOCAL axis when moving but the direction ball moves in is relative to world orientation
	float pitch, yaw, roll;
	// The 3 Color Components of this object
	float mRed, mGreen, mBlue;
	// A Time value that determines how long the collision should be ignored before being read again
	float m_fTimeToIgnoreCollision;
	// Pointer to a 3DS model
	SmartPtr<Model_3DS> mModel;
	// Pointer to a BoundingSphere for collision detection
	SmartPtr<BoundingSphere> mBoundingSphere;
	// Pointer to a Bounding box for collision detection
	SmartPtr<BoundingBox> mBoundingBox;	
	// Pointer to the current gameworld that owns this object
	GameWorld* mWorld;							

	// Constructor
	Entity(const char* type) : BaseClass(),
							   mType(type) , 
							   mPosition(Vec3(0, 0, 0)),
							   mScale(1),
							   mRadius(0),
							   m_mass(1),
							   mAlive(false),
							   m_bIgnoreCollision(false),
							   m_worldMatrix(Matrix4::IDENTITY), 
							   m_orientation(Quaternion::IDENTITY), 
							   m_rotation(Quaternion::IDENTITY),
							   mForward(0, 0, 1), mUp(0, 1, 0), mRight(1, 0, 0), 
							   m_eulerOrient(0, 0, 0),
							   m_maxTurnRate(0), pitch(0), yaw(0), roll(0), m_fTimeToIgnoreCollision(0),
							   mRed(1.0), mBlue(1.0), mGreen(1.0)
	{}

	Entity(const char* type, Vec3 pos,float scale, float rad) : BaseClass(),
																 mType(type),
																 mPosition(pos),
																 mScale(scale),
																 mRadius(rad),
																 m_mass(1),
																 mAlive(false),
																 m_bIgnoreCollision(false),
																 m_worldMatrix(Matrix4::IDENTITY), 
																 m_orientation(Quaternion::IDENTITY), 
																 m_rotation(Quaternion::IDENTITY),
																 mForward(0, 0, 1), mUp(0, 1, 0), mRight(1, 0, 0), 
																 m_eulerOrient(0, 0, 0),
																 m_maxTurnRate(0), pitch(0), yaw(0), roll(0), m_fTimeToIgnoreCollision(0),
																 mRed(1.0), mBlue(1.0), mGreen(1.0)
	{}

	Entity(const char* type, int& id) : BaseClass(id), 
										mType(type),
									    mPosition(Vec3(0, 0, 0)),
							            mScale(1),
							            mRadius(0),
										m_mass(1),
							            mAlive(false),
										m_bIgnoreCollision(false),
										m_worldMatrix(Matrix4::IDENTITY), 
										m_orientation(Quaternion::IDENTITY), 
										m_rotation(Quaternion::IDENTITY),
										mForward(0, 0, 1), mUp(0, 1, 0), mRight(1, 0, 0), 
										m_eulerOrient(0, 0, 0),
										m_maxTurnRate(0), pitch(0), yaw(0), roll(0), m_fTimeToIgnoreCollision(0),
										mRed(1.0), mBlue(1.0), mGreen(1.0)
	{
	}

	Entity(const char* type, int& id, Vec3 pos,float scale, float rad) : BaseClass(id), 
																 mType(type),
																 mPosition(pos),
																 mScale(scale),
																 mRadius(rad),
																 m_mass(1),
																 mAlive(false),
																 m_bIgnoreCollision(false),
																 m_worldMatrix(Matrix4::IDENTITY), 
																 m_orientation(Quaternion::IDENTITY), 
																 m_rotation(Quaternion::IDENTITY),
																 mForward(0, 0, 1), mUp(0, 1, 0), mRight(1, 0, 0), 
																 m_eulerOrient(0, 0, 0),
																 m_maxTurnRate(0), pitch(0), yaw(0), roll(0), m_fTimeToIgnoreCollision(0),
																 mRed(1.0), mBlue(1.0), mGreen(1.0)
	{
	}
};

#endif 