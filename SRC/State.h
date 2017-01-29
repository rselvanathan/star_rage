/*
A Abstract base class that defines a interface for a entity state

Author : Mat Buckland (fup@ai-junkie.com)

Referenced From his Book - Programming Game AI by Example 
*/

#ifndef __STATE_H__
#define __STATE_H__

struct Telegram;

template <class entity_type>
class State
{
public:
	virtual ~State(){}

	// Execute when state is entered
	virtual void EnterState(entity_type*) =0;
	// Normal state updated function
	virtual void ExecuteState(entity_type*) = 0;
	// this will execute when state has exited
	virtual void ExitState(entity_type*) = 0;
	// this executes if the agent receives a message from the dispatcher
	virtual bool OnMessage(entity_type*, const Telegram&) =0;
};

#endif