/*
This class contains a very accurate timer, which uses the FrameTimer class
to obtain the milliseconds passes since this class was called. It contains a list of 
listeners which will send a message to them when the time changes

Author : Romesh Selvanathan
*/

#ifndef __GAMETIME_H__
#define __GAMETIME_H__

#include "ITimerListener.h"
#include "FrameTimer.h"

class GameTime
{
public:
	GameTime(){}
	
	// Initialise the GameTime class by initialising a frametimer which also has functionality to return accurate
	// time elapsed since this method is called
	void Init() {
		m_listeners.clear(); 
		m_AccurateTimer.init(60);
	}

	void AddListener(ITimerListener* lptr) {m_listeners.push_back(lptr);}
	void RemoveListener(ITimerListener* lptr) {m_listeners.remove(lptr);}
	void FireElapsedTime(){
		for(TimerList::iterator it = m_listeners.begin(); it != m_listeners.end(); it++)
			(*it)->OnTimer(m_AccurateTimer.GetTime());
	}
	
private:
	FrameTimer m_AccurateTimer;

	typedef list<ITimerListener*> TimerList;
	TimerList m_listeners;
};

#endif