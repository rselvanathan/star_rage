/*
This class Dispatches messages to other "BaseClasses". This Can also be used to send messages to a groupd of entities.
Different types of messages can be sent, for example, a delayed message or a immedite message.

Author : Romesh Selvanathan

References :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/
#include "MessageDispatcher.h"
#include "Telegram.h"
#include "BaseClass.h"
#include "GameObjectType.h"
#include "Entity.h"
#include "GameWorld.h"
#include "GameUtil.h"
#include "ClassManager.h"
#include "MessageType.h"

MessageDispatcher* MessageDispatcher::mMessageDispatcher;

/* 
Method is used by both DispatchMessage and DispatchDelayedMessage
This method calls the message handling membor function of the receiving entity
 with the newly created telegram
*/
void MessageDispatcher::Discharge(BaseClass* pReceiver, const Telegram& msg)
{
	// If message is not send then return (Need to add a throw or a message to a console)
	if(!pReceiver->HandleMessage(msg))
		return;
}

/*
This function send the message to the correct class (if no delay) or stores in the message queue
to be dispatched at the correct time

References :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/
void MessageDispatcher::DispatchMessage(double delay, int sender, int receiver, int msg, void* extraInfo)
{
	// Get Pointers to the sender and receiver
	BaseClass* pTempSender = ClassManager::GetInstance()->GetClassFromID(sender);
	// If its the pointer is empty then obtain a a screen pointer
	if(pTempSender == NULL)
		pTempSender = ClassManager::GetInstance()->GetScreenFromID(sender);

	BaseClass* pTempReceiver = ClassManager::GetInstance()->GetClassFromID(receiver);
	// If its the pointer is empty then obtain a a screen pointer
	if(pTempReceiver == NULL)
		pTempReceiver = ClassManager::GetInstance()->GetScreenFromID(receiver);

	// Make sure the receiver is valid
	if(pTempReceiver == NULL)
		return;

	// Create the telegram
	Telegram tele(0, sender, receiver, msg, extraInfo);

	// If there is no delay, route telegram immediatly
	if(delay <= 0.0f)
		Discharge(pTempReceiver, tele);
	else {
		// Obtain current time
		double tempCurrentTime = m_timer.GetTime();
		// Set the Dispatch time
		tele.m_dispatchTime = tempCurrentTime + delay;
		// Add it into the PQ.
		PriorityQ.insert(tele);
	}
}

/*
This function sends a message to all the correct GameObject classes, or stores them in the message queue
to be dispatched at the correct time
*/
void MessageDispatcher::DispatchMessageToGroup(double delay, Entity* sender, GameObjectType type, int msg, void* extraInfo)
{
	// Obtain the gameworld list
	list<Entity*> tempFind = GameWorld::GetInstance()->GetGameObjects();
	// A vector of receivers
	vector<Entity*> SendMessageTo;
	// Find all the receiver that are of that ype specified
	for(list<Entity*>::iterator it = tempFind.begin(); it != tempFind.end(); it++)
	{
		if((*it)->GetType() == type)
			SendMessageTo.push_back((*it));
	}

	// IF the receiver list is empty return
	if(SendMessageTo.empty())
		return;

	// Send message to all the receivers
	for(vector<Entity*>::iterator it = SendMessageTo.begin(); it != SendMessageTo.end(); it++) {
		// Create the telegram
		Telegram tele(0, sender->GetID(), (*it)->GetID(), msg, extraInfo);

		// If there is no delay, route telegram immediatly
		if(delay <= 0.0f)
			Discharge((*it), tele);
		else {
			// Obtain current time
			double tempCurrentTime = m_timer.GetTime();
			// Set the Dispatch time
			tele.m_dispatchTime = tempCurrentTime + delay;
			// Add it into the PQ.
			PriorityQ.insert(tele);
		}
	}
}

/*
This dispatches all telegrams with a timestamp has expired. Any dispatched telegrams are removed
from the queue.
*/
void MessageDispatcher::DispatchDelayedMessages()
{
	// Get current Time
	double tempCurrentTime = m_timer.GetTime();

	//look into the queue to see if any telegrams need dispatching. Remove all telgrams from the front of the queue
	//that have gone past their time limit
	while(!PriorityQ.empty() &&
		(PriorityQ.begin()->m_dispatchTime < tempCurrentTime) &&
		(PriorityQ.begin()->m_dispatchTime > 0) )
	{
		// read the telegram from the front of the queue
		const Telegram& tele = *PriorityQ.begin();

		// find the recipient
		BaseClass* tempReceiver = ClassManager::GetInstance()->GetClassFromID(tele.m_receiver);

		// send the telegram to the receiver
		Discharge(tempReceiver, tele);

		// remove it from the queue
		PriorityQ.erase(PriorityQ.begin());
	}
}