#ifndef __IPLAYERLISTENER_H__
#define __IPLAYERLISTENER_H__

class IPlayerListener
{
public:
	virtual void OnHealthChanged(int health) = 0;
	virtual void OnEnergyChanged(float energy) = 0;
};

#endif