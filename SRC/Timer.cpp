// The cpp File for the Timer class
// Author : Romesh Selvanathan
// Date : 16/12/2010

// Referenced From 2nd Year C++ Project

#include <conio.h>
#include <time.h>
#include <stdio.h>
#include "timer.h"
#include "GameWorld.h"

// Constructor
Timer::Timer()
{
}

// Destructor
Timer::~Timer()
{
}

// intialise the values of the object and create a text output object
bool Timer::init()
{
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
	countdown_time = 0;
	
	start();

	return true;

}

// Start the timer
void Timer::start()
{
	if(!running)
	{
		if(resetted)
		{
			beg = (unsigned long) clock();
		}
		else 
			beg -= end - (unsigned long) clock();
			

		running = true;
		resetted = false;
	}
}

// Stop the timer
void Timer::stop()
{
	if(running)
	{
		end = (unsigned long) clock();
		running = false;
	}
}

// Reset the Timer
void Timer:: reset()
{
	bool wasRunning = running;
	if(wasRunning)
		stop();

	resetted = true;
	beg = 0;
	end = 0;
	if (wasRunning)
		start();
}

// is the timer still running?
bool Timer::isRunning()
{
	return running;
}

// Return the time on the timer
unsigned long Timer::countdown()
{
	if(running){
		return (countdown_time-((unsigned long) clock() - beg)/CLOCKS_PER_SEC);
		//double diff = difftime(end, beg);
		//return diff;
	}
	else 
		return (end - beg)/CLOCKS_PER_SEC;
}

// Return the countdown time on the timer
unsigned long Timer::getTime()
{
	if(running){
		return (((unsigned long) clock() - beg)/CLOCKS_PER_SEC);
		//double diff = difftime(end, beg);
		//return diff;
	}
	else 
		return (end - beg)/CLOCKS_PER_SEC;
}

// is the timer over?
bool Timer::isOver(unsigned long seconds)
{
	return seconds >=countdown();
}