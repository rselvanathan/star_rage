/*
This class Dispatches messages to other "BaseClasses". This Can also be used to send messages to a groupd of entities.
Different types of messages can be sent, for example, a delayed message or a immedite message.

Author : Romesh Selvanathan

References :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/
#ifndef __MESSAGEDISPATCHER_H__
#define __MESSAGEDISPATCHER_H__

#include "Telegram.h"
#include "GameUtil.h"
#include "FrameTimer.h"

class BaseClass;
class Entity;
class GameObjectType;

// Makes code easier to read
const int NO_INFO = 0;
const double NO_DELAY = 0;

class MessageDispatcher
{
public:
	// This is a singleton class
	static MessageDispatcher* GetInstance(){
		if(mMessageDispatcher == NULL)
			mMessageDispatcher = new MessageDispatcher();
		return mMessageDispatcher;
	}

	// Send a message to another class
	void DispatchMessage(double delay, int sender, int receiver, int msg, void* extraInfo);
	// Send a message to a group of entities - can be used by game objects i.e enemies to communicate
    void DispatchMessageToGroup(double delay, Entity* sender, GameObjectType type, int msg, void* extraInfo);
	// Send out any Delayed messages. This method is called each time though the main game loop
	// in "GameSession"
	void DispatchDelayedMessages();

private:
	MessageDispatcher(){m_timer.init(60);}	// Start the timer in the Constructor
	static MessageDispatcher* mMessageDispatcher;

	// set is used for the delayed messages because it does automatic sorting
	// and avoids duplicates. Messages will be sorted by their dispatch time
	set<Telegram> PriorityQ;

	// Method is used by both DispatchMessage and DispatchDelayedMessage
	// This method calls the message handling membor function of the receiving entity
	// with the newly created telegram
	void Discharge(BaseClass* pReceiver, const Telegram& msg);

	FrameTimer m_timer;
};

#endif