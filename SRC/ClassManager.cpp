/*
A Class that manages and keeps track of all the "BaseClasses" - Used by MessageDispatcher class

Author : Romesh Selvanathan

References :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/
#include "ClassManager.h"
#include "GameUtil.h"
#include "BaseClass.h"

// For easy access to the singleton
ClassManager* ClassManager::mClassManager;

// Return Pointer to the entity with given ID
BaseClass* ClassManager::GetClassFromID(int id) const
{
	// find the entity
	ClassMap::const_iterator it = m_classMap.find(id);

	// check wheter entity is a member of the map
	//assert((it != m_entityMap.end()) && "<ClassManager::GetEntityFromID>:invalid ID");
	if(it == m_classMap.end())
		return NULL;

	// Return the pointer
	return it->second;
}

// Return Pointer to the entity with given ID
BaseClass* ClassManager::GetScreenFromID(int id) const
{
	// find the entity
	ClassMap::const_iterator it = m_screenMap.find(id);

	// check wheter entity is a member of the map
	//assert((it != m_entityMap.end()) && "<ClassManager::GetEntityFromID>:invalid ID");
	if(it == m_screenMap.end())
		return NULL;

	// Return the pointer
	return it->second;
}

// remove entity from the list
void ClassManager::RemoveClass(BaseClass* pClass)
{
	m_classMap.erase(m_classMap.find(pClass->GetID()));
}

// remove entity from the list
void ClassManager::RemoveScreen(BaseClass* pClass)
{
	m_screenMap.erase(m_screenMap.find(pClass->GetID()));
}

// Stores a Pointer to the entity - helps fast look up
void ClassManager::RegisterClass(BaseClass* pClass)
{
	m_classMap.insert(make_pair(pClass->GetID(), pClass));
}

// Stores a Pointer to the entity - helps fast look up
void ClassManager::RegisterScreen(BaseClass* pClass)
{
	m_screenMap.insert(make_pair(pClass->GetID(), pClass));
}
