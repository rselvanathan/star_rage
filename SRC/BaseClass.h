/*
This class will be the base class for all the classes which need to send messages to each other.

Author : Romesh Selvanathan

References :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/

#ifndef __BASECLASS_H__
#define __BASECLASS_H__

#include <assert.h>

class Telegram;

class BaseClass
{
public:
	BaseClass() : m_iID(NextValidID()) 
	{}

	BaseClass(int &customID)
	{
		// If the CustomID does not equal next valid ID then 
		// set the customID to the next valid ID
		if(customID != m_iNextValidID){
			m_iNextValidID++;
			customID = m_iNextValidID;
			m_iID = customID;
		}
		// otherwise set the current ID to the customID
		else
		{
			m_iID = customID;
			m_iNextValidID++;
		}
	}

	// All classes can communicate using messages. They are sent using the MessageDispatcher class
	virtual bool HandleMessage(const Telegram& msg) = 0;

	const int GetID() const {return m_iID;}
	static int GetNextValidID() {return m_iNextValidID;}
	static void ResetNextValidID(){m_iNextValidID = 0;}

private:
	// A Unique Identifier number for every Entity
	int m_iID;
	// this is the next valid ID - each time a Entity is instantiated this is updated
	// So any time a new one is created this will go up
	static int m_iNextValidID;
	// this must be called inside the Constructor to make sure the ID is set correctly. This
	// verified that the value passed is greater or equal to the next valid ID (above)
	void SetID(int val){
		//make sure the id is equal to or greater than the next available ID
		assert( (val >= m_iNextValidID) && "<Entity::SetID>: invalid ID");

		m_iID = val;

		m_iNextValidID = m_iID + 1;
	}
	//used by the constructor to give each entity a unique ID
	int NextValidID(){
		//static int NextID = 0; 
		m_iNextValidID++; 
		return m_iNextValidID;
	}
};

#endif