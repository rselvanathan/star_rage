/*
An interface class that allows classes to listen to keyboard character updates

Author : Romesh Selvanathan
Date : 03/05/12
*/

#ifndef __IKEYCHARLISTENER_H__
#define __IKEYCHARLISTENER_H__

class IKeyCharListener
{
public:
	virtual void OnCharKeyDown(WPARAM wchar) = 0;
};

#endif