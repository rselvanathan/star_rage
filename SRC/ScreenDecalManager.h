/*
This class deals with the screen decals that will pop up in the game. It will listen to any messages and activate the appropriate decals,
and also update the time for each of the decals in available.

Author : Romesh Selvanathan
Date : 2/04/12
*/

#ifndef _SCREENDECALMANAGER_H__
#define _SCREENDECALMANAGER_H__

#include "ITimerListener.h"
#include "BaseClass.h"
#include "GameUtil.h"

class Decal_Blood;
class Decal_BrokenGlass;

class ScreenDecalManager : public BaseClass, public ITimerListener
{
public:
	// Constructor
	ScreenDecalManager();
	// Destructor
	~ScreenDecalManager();

	// Initialise the Decals
	void Init();

	// Update the Decals
	void Update();
	// Render the Decals
	void Render();

	// Handle the messages and activate the appropriate decals
	bool HandleMessage(const Telegram& msg);

	//--------------------------ITimerListener-------------------------------------
	void OnTimer(float milliseconds);
	//-----------------------------------------------------------------------------
private:
	SmartPtr<Decal_BrokenGlass> m_pBrokenGlass;			// Pointer to a BrokenGlass Decal
	SmartPtr<Decal_Blood> m_pBlood;						// Pointer to a Blood Decal
};

#endif