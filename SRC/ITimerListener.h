#ifndef __ITIMERLISTENER_H__
#define __ITIMERLISTENER_H__

class ITimerListener
{
public:
	virtual void OnTimer(float milliseconds) = 0;
};

#endif