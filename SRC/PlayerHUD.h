/* 
The PlayerHUD shows all the required HUD components for the player.

last Edit - 22.01.12 - Added Energy bar

Author : Romesh Selvanathan
*/

#ifndef __PLAYERHUD_h__
#define __PLAYERHUD_H__

#include "GL2DBox.h"
#include "GameUtil.h"
#include "IPlayerListener.h"
#include "IScoreListener.h"

class GLText;

class PlayerHUD : public IPlayerListener, public IScoreListener
{
public:
	PlayerHUD();
	~PlayerHUD();

	void Reset() {m_score = 0;}

	////////////////////////////////IPLAYERLISTENER///////////////////////////////
	void OnHealthChanged(int health);
	void OnEnergyChanged(float energy);
	///////////////////////////////////////////////////////////////////////////// 
	////////////////////////////////ISCORELISTENER///////////////////////////////
	void OnScoreChanged(int score);
	/////////////////////////////////////////////////////////////////////////////

	void Render();

private:
	GL2DBox aim;					// Box to show the aim
	GL2DBox mHealthHud;				// The Health and Energy Box
	GL2DBox testMask;				// the mask which will mask away the texture, not allowing it to besee through
	GL2DBox mHealth;				// The Health bar
	GL2DBox mEnergy;				// The energy bar
	SmartPtr<GLText> mScoreText;	// Text for score
	int m_score;					// the score itself
};

#endif