// References : http://www.cplusplus.com/forum/beginner/317/, http://www.cplusplus.com/reference/clibrary/ctime/clock/ ,
// The timer class is just a class that creates a timer Object. This class behaves
// a bit lyk stop watch in terms of its, stop, start, countdown, reset functionality.
// Author: Rom esh Selvanathan
// Date : 16/12/2010

// Referenced From 2nd Year C++ Project

/*
Edit = 01/03/2012 - Added A list of listeners which will listen to the time update
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "GameUtil.h"

class Timer
{
public:
	// Constructor
	Timer();
	// Destructor
	~Timer();
	
	// intialise the values of the object and create a text output object 
	bool init();
	// Start the timer
	void start();
	// Stop the timer
	void stop();
	// Reset the Timer
	void reset();
	// is the timer still running?
	bool isRunning();
	// Return the time on the timer
	unsigned long getTime();
	// Return the countdown time on the timer
	unsigned long countdown();
	// is the timer over?
	bool isOver(unsigned long seconds);
	// Set the countdown time
	void SetCountdown(int seconds){ countdown_time = seconds; }

private:
	bool resetted;		// is the timer resetted?
	bool running;		// is the timer still running?
	unsigned long beg;	// time beginning
	unsigned long end;	// time end
	int countdown_time;
};

#endif