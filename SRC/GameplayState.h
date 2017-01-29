/*
This is the Gameplay screen that will initialize all the game play elements that are reqired the game screen.

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __GAMEPLAYSTATE_H__
#define __GAMEPLAYSTATE_H__

#include "GameState.h"
#include "GameUtil.h"
#include "GLFog.h"
#include "GLLight.h"
#include "PlayerManager.h"
#include "ScoreManager.h"
#include "ScreenDecalManager.h"
#include "Octree.h"
#include "Radar.h"
#include "IPlayerListener.h"
#include "IScoreListener.h"
#include "GameTime.h"
#include "FrameTimer.h"

#include "IButtonListener.h"
#include "IMousePosListener.h"

class GLText;
class Texture;
class InputManager;
class Shape;
class TestSquare;
class Spaceship;
class SpaceStation;
class Camera;
class Skybox;
class Asteroid;
class PlayerHUD;
class SmallEnemy;
class BigEnemy;
class HardEnemy;
class EnemyGenerator;
class StarField;

class PowerUp;
class OctreeTest;
class Point_Sprite;
class ExplosionSmokeParticle;

#define MAX_ASTEROIDS 50

class GameplayState : public GameState, public IPlayerListener, public IScoreListener, public IButtonListener,
					  public IMousePosListener
{
public:
	static GameplayState* GetInstance()
	{
		if(mGameplayState == NULL)
			mGameplayState = new GameplayState();
		return mGameplayState;
	} 
	virtual ~GameplayState();

	void Init(GameStateManager* mGameState);
	void Exit(){}
	void Pause(){}
	void Resume();

	void Update(float dt);
	virtual void Render();
	bool HandleMessage(const Telegram& msg){return false;}

	/////////////////////////////////////////IPLAYERLISTENER//////////////////////////////////////
	void OnHealthChanged(int health);
	void OnEnergyChanged(float energy);
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////ISCORELISTENER//////////////////////////////////////
	void OnScoreChanged(int score);
	//////////////////////////////////////////////////////////////////////////////////////////////

	//--------------------------------------IButtonListener--------------------------------------
	virtual void OnPausePress();
	virtual void OnGodModePress();
	virtual void OnCheatWavePress();
	// States
	virtual void OnMoveForwardPress();
	virtual void OnMoveBackPress();
	virtual void OnTurnLeftPress();
	virtual void OnTurnLeftRelease();
	virtual void OnTurnRightPress();
	virtual void OnTurnRightRelease();
	virtual void OnShootPress();
	virtual void OnSprintPress();
	virtual void OnSprintRelease();
	//-------------------------------------------------------------------------------------------

	//-------------------------------------IMousePosListener-------------------------------------
	void OnMouse(int x, int y);
	void OnMouseStop();
	//-------------------------------------------------------------------------------------------

	// Load the and set up all the different asteroids
	void LoadAsteroids();				

	const int GetScore() const {return m_score;}

private:
	static GameplayState* mGameplayState;
	GameplayState();

	// The skybox			
	SmartPtr<Skybox> pSkybox;
	// The Player HUD
	SmartPtr<PlayerHUD> mHUD;
	// The Enemy Generator that will generate and manage all the enemies
	SmartPtr<EnemyGenerator> mGenerator;
	// The Player Spaceship
	SmartPtr<Spaceship> pSpaceship;
	// The set number of asteroids that are created in the world
	SmartPtr<Asteroid> pAsteroid[MAX_ASTEROIDS];
	// The Star field particle effect
	SmartPtr<StarField> pStarField;
	// The Player manager
	PlayerManager mPlayerManager;
	// The score manager
	ScoreManager mScoreManager;
	// The Screen Decal manager
	ScreenDecalManager mDecalManager;
	// Light that will be used to light up the world
	GLLight mLight;
	// The Game time, which will be used to update all objects that rely on it
	GameTime mGameTime;
	// The Fade out effect square
	GL2DBox flash;						
	// The radar system
	Radar rad;

	// flag to see if sprinting is enabled
	bool sprinting;	
	// To show whether enemy is alive or not on the screen
	bool m_bUnkillable;
	// To disable the sprint if energy is 0
	bool m_energyDepleted;
	// A flag to show whether or not the player has died
	bool m_gameover;
	// The alpha value that will allow game to fade out
	float gameOverAlpha;
	int m_score;						// Current score of the player - to be passed onto the highscore state

	// Values to be passed onto the player
	float roll, pitch, heading;
};

#endif