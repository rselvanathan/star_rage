/*
This is a generic class that will generate a Screen Decal effect, i.e Blood Splatter. This class
requires the texture, time till it dies and the initial alpha value set for each decall type.
The user simply has to set the three values and update the current time value, in order for the decal
to dissappear.

Author : Romesh Selvanathan
Date : 03/05/12
*/

#ifndef __SCREENDECAL_H__
#define __SCREENDECAL_H__

#include "GL2DBox.h"

class ScreenDecal
{
public:
	// Constructor, that requires texture, time till it dissapears, and the initial alpha
	ScreenDecal(unsigned int texture, float timeTillDead, float alpha);
	// Constructor, that requires time till it dissapears, and the initial alpha
	ScreenDecal(float timeTillDead, float alpha);
	// Reset the Decal
	void Reset();
	// Mutator methods
	void SetActive() {m_bActive = true; m_fCountdown = m_fCurrentTime;}
	void SetInactive() {m_bActive = false;}
	void SetCurrentTime(float milliseconds) {m_fCurrentTime = milliseconds;}
	void SetTexture(unsigned int texture) {m_decalQuad.SetTexture(texture);}
	// Update the decal
	virtual void Update();
	// Render the decal
	virtual void Render();

protected :
	// The 2D orthographic box
	GL2DBox m_decalQuad;
	// The initial alpha value
	float m_fAlpha;
	// the time till the decal dissapears
	float m_fTimeTillDead;
	// the countdown to check against
	float m_fCountdown;
	// the current time since game started - needs to be updated
	float m_fCurrentTime;
	// is the Decal active?
	bool m_bActive;
};

#endif