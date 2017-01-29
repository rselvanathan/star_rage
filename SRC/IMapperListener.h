/*
An interface class that allows classes to listen to the Input mapper updates

Author : Romesh Selvanathan
Date : 10/12
*/
#ifndef __IMAPPERLISTENER__
#define __IMAPPERLISTENER__

struct MappedInput;

class IMapperListener
{
public:	
	virtual void OnInputUpdate(MappedInput& input) = 0;
	virtual void OnInputReleaseUpdate(MappedInput& input) = 0;
};

#endif 