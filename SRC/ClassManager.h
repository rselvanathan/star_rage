/*
A Class that manages and keeps track of all the "BaseClasses" - Used by MessageDispatcher class

Author : Romesh Selvanathan

References :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/

#ifndef __CLASSMANAGER_H__
#define __CLASSMANAGER_H__

#include "GameUtil.h"
#include <map>

class BaseClass;

class ClassManager
{
public:
	// Singleton class 
	static ClassManager* GetInstance()
	{
		if(mClassManager == NULL)
			mClassManager = new ClassManager();
		return mClassManager;
	} 
	// Stores a Pointer to the entity - helps fast look up
	void RegisterClass(BaseClass* newClass);
	void RegisterScreen(BaseClass* newScreen);
	// Return Pointer to the entity with given ID
	BaseClass* GetClassFromID(int id) const;
	BaseClass* GetScreenFromID(int id) const;
	// remove entity from the list
	void RemoveClass(BaseClass* pEntity);
	void RemoveScreen(BaseClass* pEntity);
	// Clear all entities in the map
	void Reset() {m_classMap.clear();}

private:
	typedef map<int, BaseClass*> ClassMap;

private:
	ClassManager(){}
	static ClassManager* mClassManager;

	// This helps quick look up for entities , which are stored in a map. So pointers to entities
	// are cross referenced by their identifying number
	ClassMap m_classMap;
	// Another map for the screen states - Seperating from the other map, as the normal map will be reset
	// whenever the game is not active, whereas the screens will always be active.
	ClassMap m_screenMap;
};

#endif