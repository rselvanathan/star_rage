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

#include "Enemy.h"
#include "Enemy_Targeting.h"
#include "GameUtil.h"
#include "Entity.h"

Enemy_Targeting::Enemy_Targeting(Enemy* enemy) : m_pOwner(enemy),
												 m_pCurrentTarget(0)
{}

void Enemy_Targeting::Update()
{
	// Figure out which potential target is closest and set that as the current Target of the enemy taking in certain factors
	// i.e Health
	float BestSoFar = MinFloat;
	// Obtain the Potential Targets the owner of this class can have
	vector<Entity*> temp = m_pOwner->GetPotentialTargetArr();
	// If the vector is not empty
	if(!temp.empty()){
		// for every entity
		for(vector<Entity*>::iterator it = temp.begin(); it != temp.end(); it ++)
		{
			// Obtain the position on that entity
			Vec3 targetPos = (*it)->GetPosition();
			// Obtain the distance to that object from the owner
			Vec3 direction = targetPos - m_pOwner->GetPosition();
			float distance = direction.Length();
			// temporary result 
			float result;
			// if the object is a energy powerup then calculate the energy desirability
			if((*it)->GetType() == GameObjectType("powerupenergy"))
				// current and maximum distance provided as parameters
				result = m_pOwner->CalcEnergyDesirability(distance, 100);
			// if the object is a health powerup then calculate the health desirability
			else if((*it)->GetType() == GameObjectType("poweruphealth")){
				// current and maximum distance provided as parameters
				result = m_pOwner->CalcHealthDesirability(distance, 100);
			}
			// if the object is the player then calculate the player desirability
			else {
				// current and maximum distance provided as parameters
				result = m_pOwner->CalcPlayerDesirability(distance, 50);
			} 
			// If the Best result so far is lower than the current result
			if(BestSoFar < result){
				// Then equal the best result to the current result
				BestSoFar = result;
				// set the new enemy target to the most desriable object
				m_pOwner->SetTargetObject(static_cast<MovingEntity*>(*it));
			} 
		}
	}
}