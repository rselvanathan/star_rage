/*
A State Machine class that manages all the states of a specific type of entity.

Author : Mat Buckland (fup@ai-junkie.com)

Referenced From his Book - Programming Game AI by Example 
*/

#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "GameUtil.h"
#include "State.h"
#include "Telegram.h"

template <class entity_type>
class StateMachine
{
public:
	StateMachine(entity_type* owner) : m_pOwner(owner),
									   m_pCurrentState(NULL),
									   m_pPreviousState(NULL),
									   m_pGlobalState(NULL)
	{}

	virtual ~StateMachine(){}

	// Methods used to initialize the Finite State Machine
	void SetCurrentState(State<entity_type>* s) {m_pCurrentState = s;}
	void SetGlobalState(State<entity_type>* s) {m_pGlobalState = s;}
	void SetPreviousState(State<entity_type>* s) {m_pPreviousState = s;}
	// Accessor Methods
	State<entity_type>* GetCurrentState() const {return m_pCurrentState;}
	State<entity_type>* GetGlobalState() const {return m_pGlobalState;}
	State<entity_type>* GetPreviousState() const {return m_pPreviousState;}

	// Update the State Machine
	void Update() const
	{
		// if global state exists then execute it 
		if(m_pGlobalState)
			m_pGlobalState->ExecuteState(m_pOwner);
		// if the current state exists then execute it
		if(m_pCurrentState)
			m_pCurrentState->ExecuteState(m_pOwner);
	}
	
	bool HandleMessage(const Telegram& msg) const 
	{
		// first see if the current state is valid and it can handle the message
		if(m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
			return true;

		// if not and if a global state has been implemented, send the message to the global state
		if(m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
			return true;

		return false;
	}

	// Change to a new State
	void ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState && "<StateMachine:ChangeState>trying to assign null state to current");

		// keep a record of a previous state
		m_pPreviousState = m_pCurrentState;
		//call the exit method of the existing state
		m_pCurrentState->ExitState(m_pOwner);
		// change the current state to the new state
		m_pCurrentState = pNewState;
		// call the entry method of the new state
		m_pCurrentState->EnterState(m_pOwner);
	}

	// Change back to the previous state
	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	// return if the current state's type is equal to the type of the class
	// passed as a parameter
	// http://www.cplusplus.com/reference/std/typeinfo/type_info/ - for what typeid is
	bool IsInState(const State<entity_type>& st)const
	{
		if(typeid(*m_pCurrentState) == typeid(st))
			return true;
		return false;
	}

private:
	// pointer to the entity that owns this instance
	entity_type* m_pOwner;
	// current state of the entity
	State<entity_type>* m_pCurrentState;
	// record of the last state of the agent was in
	State<entity_type>* m_pPreviousState;
	// this is called everytime the Finite State machine is updated
	State<entity_type>* m_pGlobalState;
};

#endif