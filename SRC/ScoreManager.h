#ifndef __SCOREMANAGER_H__
#define __SCOREMANAGER_H__

/*
This class updates the score for the player and will update and send messages to all its listeners.

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#include "IGameWorldListener.h"
#include "IScoreListener.h"
#include "Entity.h"
#include "GameObjectType.h"
#include "GameUtil.h"

class ScoreManager : public IGameWorldListener
{
public:
	/* Constructor */ 
	ScoreManager() : score(0), scoreMultiplier(1) {}

	/* Reset this object */ 
	void Reset() {score = 0; scoreMultiplier = 1;}

	////////////////////////////////////////////IGAMEWORLDLISTENER//////////////////////////////////////
	void OnObjectAdded(GameWorld* world, Entity* object){}
	void OnObjectRemoved(GameWorld* world, Entity* object)
	{
		// If coin is removed from the world increase the score by value
		if(object->GetType() == GameObjectType("smallenemy")){
			score += (10*scoreMultiplier);
			FireScoreChanged();
		}
		// if enemy is removed from the world then increase score as well as the multiplier by a value
		if(object->GetType() == GameObjectType("bigEnemy")){
			score += (30*scoreMultiplier);
			//scoreMultiplier = 3;
			FireScoreChanged();
		}
		// if enemy is removed from the world then increase score as well as the multiplier by a value
		if(object->GetType() == GameObjectType("hardenemy")){
			score += (60*scoreMultiplier);
			//scoreMultiplier = 3;
			FireScoreChanged();
		}
	}

	/* Add A listener to the list */ 
	void AddListener(SmartPtr<IScoreListener> ptr) {mListeners.push_back(ptr);}
	/* Fire messages to all its Listeners */ 
	void FireScoreChanged(){
		for(ScoreListenerList::iterator it = mListeners.begin(); it != mListeners.end(); it++)
			(*it)->OnScoreChanged(score);
	}
private:	
	int score;							// current score
	int scoreMultiplier;				// current score multiplier value

	typedef list< SmartPtr<IScoreListener> > ScoreListenerList;
	ScoreListenerList mListeners;
};

#endif