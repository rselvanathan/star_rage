/*
This interface updates all its listeners about changes to the score.

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __ISCORELISTENER_H__
#define __ISCORELISTENER_H__

class IScoreListener
{
public:
	virtual void OnScoreChanged(int score) = 0;
};

#endif