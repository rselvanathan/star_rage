#ifndef __SPACESTATION_H__
#define __SPACESTATION_H__

#include "Entity.h"

class SpaceStation : public Entity
{
public:
	SpaceStation();
	virtual ~SpaceStation() {Entity::~Entity();}

	bool Init(){return true;}
	void Reset();

	void Update(float dt){}
	bool HandleMessage(const Telegram& msg){return true;}

	//-------------------------------ITimerListener-------------------------------------
	void OnTimer(float milliseconds);
	//----------------------------------------------------------------------------------

private:

};

#endif