/*
This class controls and updates all the values related to the player. It also sends events to the decal manager
whenever the player loses health.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#ifndef __PLAYERMANAGER_H__
#define __PLAYERMANAGER_H__

#include "GameUtil.h"
#include "Entity.h"
#include "GameObjectType.h"
#include "IPlayerListener.h"
#include "ClassIDList.h"
#include "BaseClass.h"
#include "Telegram.h"
#include "MessageType.h"
#include "MessageDispatcher.h"

#define TIME_GAP 200

class PlayerManager : public BaseClass
{
public:
	/* Constructor */ 
	PlayerManager() : BaseClass(ClassIDList::GetInstance()->class_PlayerManager)
	{mHealth = 100; mEnergy = 100; energyUsed = false; timeTillEnergyIncrease = TIME_GAP;}

	/* Reset this object */ 
	void Reset() 
	{
		mHealth = 100;
		mEnergy = 100;
		energyUsed = false;
		timeTillEnergyIncrease = TIME_GAP;
		FireHealthChanged();
		FireEnergyChanged();
		mListeners.clear();
	}

	//--------------------------------BaseClass-----------------------------------------
	// If the message is that the health power up was picked up then increase health
	// if the the energy power up was picked , increase the energy
	bool HandleMessage(const Telegram& msg){
		int tempMsg = msg.m_message;
		
		switch (tempMsg){
		case msg_playerPickedHealth :
			mHealth += 40;
			Clamp(mHealth, 0, 100);
			FireHealthChanged();
			break;

		case msg_playerPickedEnergy :
			mEnergy += 20;
			Clamp(mEnergy, 0, 100);
			FireEnergyChanged();
			break;

		case msg_playerHasBeenAttacked : 
			{
				mHealth -= 8;
				Clamp(mHealth, 0, 100);


				if(mHealth <= 20)
					MessageDispatcher::GetInstance()->DispatchMessageA(0, 
						ClassIDList::GetInstance()->class_PlayerManager,
						ClassIDList::GetInstance()->class_ScreenDecalManager,
						msg_decalBrokenGlass, NULL);
				else
					MessageDispatcher::GetInstance()->DispatchMessageA(0, 
						ClassIDList::GetInstance()->class_PlayerManager,
						ClassIDList::GetInstance()->class_ScreenDecalManager,
						msg_decalBlood, NULL);

				FireHealthChanged();
			}
			break;

		case msg_playerNormalShot :
			{
				energyUsed = true;
				mEnergy -= 0.6f;
				Clamp(mEnergy, 0, 100);

				FireEnergyChanged();
			}
			break;

		case msg_playerSprint_On:
			{
				energyUsed = true;
				mEnergy -= 0.1;
				Clamp(mEnergy, 0, 100);

				FireEnergyChanged();
			}
			break;

		case msg_playerCollidesAsteroid :
			mHealth = 0;

			FireHealthChanged();
			break;
		}

		return true;
	}
	//----------------------------------------------------------------------------------

	void Update(){
		// If the energy has not been used recently start incrementing slowly again
		if(energyUsed){
			timeTillEnergyIncrease -= 1;
			if(timeTillEnergyIncrease <= 0){
				timeTillEnergyIncrease = TIME_GAP;
				energyUsed = false;
			}
		}
		if(!energyUsed)
			mEnergy += 0.02;
		Clamp(mEnergy, 0, 100);
		FireEnergyChanged();
	}

	int Test() {return mListeners.size();}

	/* Add A listener to the list (Using Raw Pointer may caus memory leak) */ 
	void AddListener(IPlayerListener* listener){mListeners.push_back(listener);}
	/* Fire the Change in health to all its Listeners */ 
	void FireHealthChanged()
	{
		for(PlayerListenerList::iterator lit = mListeners.begin(); lit != mListeners.end(); ++lit)
			(*lit)->OnHealthChanged(mHealth);
	}
	/* Fire the Change in Energy to all its Listeners */ 
	void FireEnergyChanged()
	{
		for(PlayerListenerList::iterator lit = mListeners.begin(); lit != mListeners.end(); ++lit)
			(*lit)->OnEnergyChanged(mEnergy);
	}

private:
	int mHealth; // The Health value set to 100
	float mEnergy; // The Energy value set to 100

	bool energyUsed;				// Has Energy been used
	int timeTillEnergyIncrease;		// Time gap till energy increase occurs

	typedef list< IPlayerListener* > PlayerListenerList;
	PlayerListenerList mListeners;
};

#endif 