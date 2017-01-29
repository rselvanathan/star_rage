/*
This is base of the turret, which is static and has no functions except to be rendered.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#include "TurretBase.h"
#include "Entity.h"
#include "GameUtil.h"
#include "BoundingSphere.h"
#include "GameWorld.h"
#include "Model_3DS.h"
#include "CRandom.h"


TurretBase::TurretBase(int placement) : Entity("TurretBase")
{
	mRadius = 3;
	//mBoundingSphere = new BoundingSphere(this, mRadius);

	mScale = 0.03f;

	SmartPtr<Model_3DS> model = new Model_3DS();
	model->Load("3DModels//Enemy//Turrets//TurretBase.3DS");
	SetModel(model);

	m_worldMatrix = Matrix4::IDENTITY;

	if(placement == BOTTOM)
		roll = 180;
}

TurretBase::~TurretBase()
{Entity::~Entity();}

