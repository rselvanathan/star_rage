/*
The Enemy Targeting mechanism, that allows the AI to choose its target. This class
can be used for any type of enemy. It simply requires the enemies potential target list
from which it will use the utility functions to figure out which is the most wanted
target. Then it will set the new enemy target for the enemy to chase.

Author : Romesh Selvanathan
Date : 3/05/12

References :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/

#ifndef __ENEMY_TARGETING_H__
#define __ENEMY_TARGETING_H__

#include "GameUtil.h"

class Enemy;
class Entity;

class Enemy_Targeting
{
public:
	Enemy_Targeting(Enemy* owner);

	void Update();

	const bool isTargetPresent() const {return m_pCurrentTarget != 0;}
	Entity* GetTarget() const {return m_pCurrentTarget;}

	void ClearTarget() {m_pCurrentTarget = 0;}
private:
	// The owner of this system
	Enemy* m_pOwner;

	// the current target
	Entity* m_pCurrentTarget;
};

#endif