/*
This class contains all the IDs for specific classes. Used for message dispatching

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __CLASSIDLIST_H__
#define __CLASSIDLIST_H__

// Define the NULL
#define NULL 0

class ClassIDList
{
public:
	static ClassIDList* GetInstance()
	{
		if(mClassIDList == NULL)
			mClassIDList = new ClassIDList();
		return mClassIDList;
	}

	int ent_player;					// ID for the player spaceship
	int ent_hardEnemy;				// ID for the hard enemy
	int class_PlayerManager;		// ID for the player manager
	int class_ScreenDecalManager;	// ID for the screen decal manager

private:
	ClassIDList() {}

	static ClassIDList* mClassIDList;
};

#endif