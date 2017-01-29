/** The Frame Timer class, takes a fps value and tries to animate everything based on 
that fps. This is done so that thers would be no difference in running a game
on a fast computer and running that same game on a slower one.
EDIT: Also Added a timer function where the CPU is queried for time and stored into a 64 bit
integer varaiable. From this The current time can be worked out since the program was started.
With this I can work out a constant elpased time since last update function to be used in game
Author : Romesh Selvanathan

Referenced from previous projects : Escape - Advanced Games Technology
									C++ Programming project - 2nd Year
*/
#ifndef __FRAMETIMER_H__
#define __FRAMETIMER_H__

#include "GameUtil.h"

class FrameTimer
{
public:
	FrameTimer(){} // Constructor
	~FrameTimer(){} // Deconstructor

	// Sets up the timer with a requested frame rate of the program
	void init(int fps);
	// Returns number of frames that need to be updated to maintain the fps
	const int framesToUpdate();
	// Return the current time 
	const float GetTime();
	// Returns the elapsed time since the last update
	const float GetElapsedTimeSinceLastUpdate();

private:
	// Timer Variables used for calculating elapsed time 
	__int64 frequency;								// Holds the frequency of current PC
	__int64 timer_start;							// The Timer started 
	__int64 timer_elapsed;							// time that has elapsed since the timer was started
	float resolution;								// keeps track of how many steps it takes to reach 1 millisecond

	// timer variables
	largeInt timeNow;
	largeInt timePrevious;
	largeInt timerFreq;

	float fps;
	float intervals;
	float intervalsSinceLastUpdate;
	float framesInLastSecond;
	float intervalsPerFrame;
	int Requested_FPS;
};

#endif;