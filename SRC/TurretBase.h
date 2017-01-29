/*
This is base of the turret, which is static and has no functions except to be rendered.

Author : Romesh Selvanathan
Date : 3/05/12
*/
#ifndef __TURRETBASE_H__
#define __TURRETBASE_H__

#include "GameUtil.h"
#include "Entity.h"
#include "CRandom.h"

class TurretBase : public Entity
{
public :
	TurretBase(int placement);
	virtual ~TurretBase();

	bool Init(){return true;}
	void Reset() {}

	virtual void Update(float dt){}
	bool HandleMessage(const Telegram& msg) {return false;}

	void SetWorldMatrix(Matrix4 m) {m_worldMatrix = m;}

	enum{TOP, BOTTOM};						// Determines whether the turret base will be on top or bottom

	//-------------------------------ITimerListener-------------------------------------
	virtual void OnTimer(float milliseconds){}
	//----------------------------------------------------------------------------------

private:

	Matrix4 m_worldMatrix;				// World Matrix of spaceship
};

#endif