/*
This defines a telegram which is a dat structure that records info required to dispatch messages. 
Messages will be used by game entities to communicate with each other.

Referenced directly from :

Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/

#ifndef __TELEGRAM_H__
#define __TELEGRAM_H__

#include "GameUtil.h"

struct Telegram
{
	// the class that sent this message
	int m_sender;
	// the class that is meant to receive this message
	int m_receiver;
	// The Message itself (enumareted in MessageTypes.h)
	int m_message;
	// Messages can be sent immediatly or delayed, if delayed then a time stamp is sent with the message
	double m_dispatchTime;
	// Any additional info with the message (such as a Vector or Matrix)
	void* m_extraInfo;

	Telegram():m_dispatchTime(-1),m_message(-1), m_receiver(-1), m_sender(-1) {}
	Telegram(double time, int sender, int receiver, int msg, void* info = NULL) :
		m_dispatchTime(time), m_message(msg), m_receiver(receiver), m_sender(sender), 
		m_extraInfo(info) 
	{}
};

/*
The telegrams will be stored in a priority queue. Therefore the >
operator needs to be overloaded so that the queue can sort the telegrams
by their time priority. The times must be smaller than the value below only
if two telegrams are considered unique.
*/

// This value makes sure that telegrams with similar dispatch times  not to be delivered in mass
// and flooding the entity with identical messages
// This value can change depending on how many messages you want the entity to receive
const double SmallestDelay = 0.1f;

inline bool operator == (const Telegram& t1, const Telegram& t2)
{
	return (fabs(t1.m_dispatchTime - t2.m_dispatchTime) < SmallestDelay) &&
			(t1.m_sender == t2.m_sender) &&
			(t1.m_receiver == t2.m_receiver) &&
			(t1.m_message == t2.m_message);
}

inline bool  operator <(const Telegram& t1, const Telegram& t2)
{
	if(t1 == t2)
		return false;
	else
		return (t1.m_dispatchTime < t2.m_dispatchTime);
}

// A helper function for derenferencing the extra info field into the required type
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}

#endif