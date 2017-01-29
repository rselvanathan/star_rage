/*
This is the Gameplay screen that will initialize all the game play elements that are reqired the game screen.

Author : Romesh Selvanathan

Date : 3/05/12
*/
#include "GameState.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GLText.h"
#include "GameStateManager.h"
#include "PauseState.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "HighScoreState.h"
#include "Texture.h"
#include "PreLoader.h"
#include "InputManager.h"
#include "Camera.h"
#include "Skybox.h"
#include "Spaceship.h"
#include "SpaceStation.h"
#include "Graphics.h"
#include "GLFog.h"
#include "GLLight.h"
#include "PlayerHUD.h"
#include "ScoreManager.h"
#include "Asteroid.h"
#include "PowerUp.h"
#include "ParticleManager.h"
#include "PowerUpManager.h"
#include "ScreenDecalManager.h"
#include "ShootManager.h"
#include "ClassIDList.h"
#include "ClassManager.h"
#include "MessageDispatcher.h"
#include "MessageType.h"
#include "ScreenDecal.h"
#include "ScreenIDList.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include "LensFlare.h"
#include "Octree.h"
#include "Radar.h"
#include "SmallEnemy.h"
#include "BigEnemy.h"
#include "HardEnemy.h"
#include "TurretHead.h"
#include "EnemyGenerator.h"
#include "StarField.h"
#include "ExplosionSmokeParticle.h"
#include "GameTime.h"

GameplayState* GameplayState::mGameplayState;

GameplayState::GameplayState() : GameState(ScreenIDList::GetInstance()->gameplay)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

GameplayState::~GameplayState()
{
	SAFE_DELETE(pSpaceship);
	for(int i = 0; i < MAX_ASTEROIDS; i++)
		SAFE_DELETE(pAsteroid[i]);
	GameState::~GameState();
}

// Initialize all the game elements
void GameplayState::Init(GameStateManager* mGameState)
{
	// Reset the ClassManager
	ClassManager::GetInstance()->Reset();

	alpha = roll= pitch =heading = gameOverAlpha = 0.0f;
	mInactive = sprinting = m_energyDepleted = m_gameover = m_bUnkillable = false;
	mGameStateManager = mGameState;
	m_score = 0;
	// Reset the managers
	mScoreManager.Reset();
	mPlayerManager.Reset();
	mDecalManager.Init();
	GameWorld::GetInstance()->Reset();
	rad.Reset();

	// Turn this into a listner of the player manager
	mPlayerManager.AddListener(this);
	// allow this class to listen to the score manager
	mScoreManager.AddListener(this);
	// Turn the radar a listener of game world
	GameWorld::GetInstance()->AddListener(&rad);
	// Make score manager to listen game world
	GameWorld::GetInstance()->AddListener(&mScoreManager);
	// Register the classes that require to send messages to each other
	ClassManager::GetInstance()->RegisterClassA(&mPlayerManager);
	ClassManager::GetInstance()->RegisterClassA(&mDecalManager);

	// Initialise the Game Time and set the Power Up Manager and the decal manager as listeners
	mGameTime.Init();
	mGameTime.AddListener(PowerUpManager::GetInstance());
	mGameTime.AddListener(&mDecalManager);

	// Initialise the Player HUD
	if(mHUD.GetPtr() == NULL)
		mHUD = new PlayerHUD();
	mHUD->Reset();

	// Initialise the Shoot Manager
	ShootManager::GetInstance()->Init();
	// Turn the Shoot Manager into a listener of the Game world
	GameWorld::GetInstance()->AddListener(ShootManager::GetInstance());

	// Set up the Player SPaceship class
	if(pSpaceship.GetPtr() == NULL)
		pSpaceship = new Spaceship();
	pSpaceship->Reset();
	pSpaceship->SetPosition(Vec3(0.f, 0.5f, -4.f));
	// For the enemy bullets 
	GameWorld::GetInstance()->AddGameObject(pSpaceship);
	GameWorld::GetInstance()->AddCollisionEntity(pSpaceship);
	ClassManager::GetInstance()->RegisterClass(pSpaceship.GetPtr());
	mGameTime.AddListener(pSpaceship);

	// Set up all the different asteroids
	LoadAsteroids();

	// Set up the Power Up Manager and turn it into a listener of the game world
	PowerUpManager::GetInstance()->Init();
	GameWorld::GetInstance()->AddListener(PowerUpManager::GetInstance());
	
	// Set up the Star Field
	if(pStarField.GetPtr() == NULL)
		pStarField = new StarField(pSpaceship->GetPosition() + Vec3(-10, -10, 10), 50);
	pStarField->SetupParticles();

	// Set up the Enemy Generator
	if(mGenerator.GetPtr() == NULL) 
		mGenerator = new EnemyGenerator(&mPlayerManager, pSpaceship, &mGameTime);
	mGenerator->Reset();
	mGenerator->AddWave();
	GameWorld::GetInstance()->AddListener(mGenerator);
	mGameTime.AddListener(mGenerator);

	// Set up the Skybox
	if(pSkybox.GetPtr() == NULL)
		pSkybox = new Skybox();

	mPlayerManager.AddListener(mHUD.GetPtr());
	mPlayerManager.AddListener(pSpaceship.GetPtr());

	mScoreManager.AddListener(mHUD.GetPtr());

	// Set up the light
	mLight.SetAmbientColor(0.1, 0.1, 0.5, 1);
	mLight.SetDiffuseColor(0.5f, 0.5, 0.5, 1);
	mLight.SetSpecularColor(0.6, 0.6, 0.6, 1);
	mLight.SetShininess(98);
	mLight.SetPosition(0.0, 0.0, -1.0, 0.0);

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	// Reset the Particle Manager
	ParticleManager::GetInstance()->Reset();
}

void GameplayState::Resume()
{
	mInactive = false;
}

//--------------------------------------IButtonListener--------------------------------------
void GameplayState::OnPausePress() 
{
	// if the game is not over
	if(!m_gameover){
		// ESC will alllow pause state to come through
		mInactive = true;
		mGameStateManager->SetMessageActive(true);
	}
}
void GameplayState::OnGodModePress() 
{
	// if its not game over
	if(!m_gameover) {
		// Follow are cheat buttons - Q makes player invincible, and C will remove a whole wave
		if(!m_bUnkillable)
			m_bUnkillable = true;
		else m_bUnkillable = false;
	}
}

void GameplayState::OnCheatWavePress() 
{
	// if its not game over
	if(!m_gameover) {
		// Kill all the enemies on the current wave
		mGenerator->CheatWave();
	}
}

// States
void GameplayState::OnMoveForwardPress()
{
	// if its not game over
	if(!m_gameover) {
		// Move the Spaceship forward
		pSpaceship->MoveForward(Vec3(0, 0, 2));
	}
}
void GameplayState::OnMoveBackPress()
{
	// if its not game over
	if(!m_gameover) {
		// Move the SPaceship backwards
		pSpaceship->MoveBackwards(Vec3(0, 0, -2));
	}
}
void GameplayState::OnTurnLeftPress()
{
	// if its not game over
	if(!m_gameover) {
		// If not sprinting then rotate left
		if(!sprinting)
			heading = 1.5f;
	}
}

void GameplayState::OnTurnLeftRelease()
{
	// Reset the heading value
	heading = 0.0f;
}

void GameplayState::OnTurnRightPress()
{
	// if its not game over
	if(!m_gameover) {
		// If not sprinting then rotate right
		if(!sprinting)
			heading = -1.5f;
	}
}

void GameplayState::OnTurnRightRelease()
{
	// Reset the heading value
	heading = 0.0f;
}

void GameplayState::OnShootPress()
{
	// if the game has not finished and the mouse is pressed then the player must shoot a laser beam
	if(!m_gameover)
		pSpaceship->ShootBullet();
}

void GameplayState::OnSprintPress()
{
	// if its not game over
	if(!m_gameover) {
		// Spring enabled if energy is not 0
		if(!m_energyDepleted){
			sprinting = true;
			// Send a message to the player
			MessageDispatcher::GetInstance()->DispatchMessageA(0, NULL, 
				ClassIDList::GetInstance()->ent_player, msg_playerSprint_On, NULL);
			// Enable sprint in star field
			pStarField->SetSprint(true);
		}
	}
}

void GameplayState::OnSprintRelease()
{
	// if its not game over
	if(!m_gameover) {
		// Disable the sprint effect
		sprinting = false;
		pStarField->SetSprint(false);
		MessageDispatcher::GetInstance()->DispatchMessageA(0, NULL, 
			ClassIDList::GetInstance()->ent_player, msg_playerSprint_Off, NULL);
	}
}
//-------------------------------------------------------------------------------------------

//-------------------------------------IMousePosListener-------------------------------------
void GameplayState::OnMouse(int x, int y)
{
	// Update the roll value depending on the mouse value
	if(!m_gameover){
		if(!sprinting) {
			//Clamp(tempX, -50, 50);
			//roll = -tempX * 0.09f;//0.002f;
			roll = x;

			//Clamp(tempY, -50, 50);
			//pitch = -tempY * 0.12f;
			pitch = y;
		}
	}
}
//-------------------------------------------------------------------------------------------

/////////////////////////////////////////////////IPLAYERLISTENER////////////////////////////////////////////////////////////
void GameplayState::OnHealthChanged(int health)
{
	// if the cheat is not on then the game must end when the health reaches 0 
	if(!m_bUnkillable)
		if(health <= 0){
			GameWorld::GetInstance()->RemoveGameObject(pSpaceship);
			m_gameover = true;
		}
}

void GameplayState::OnEnergyChanged(float energy)
{
	// when energy reaches 0 then the energy depleted flag is turned on
	if(energy <= 0)
		m_energyDepleted = true;
	else 
		m_energyDepleted = false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////ISCORELISTENER////////////////////////////////////////////////////////////
void GameplayState::OnScoreChanged(int score)
{
	m_score = score;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameplayState::Update(float dt)
{
	if(mInactive){
		// if it is fully blended in change state
		if(alpha >= 1.0f){
			mGameStateManager->AddState(PauseState::GetInstance());
			//Pop the current Context and add the new context for the input
			InputManager::Instance()->PopContext();
			InputManager::Instance()->PushContext(L"menucontext");
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->AddMenuButtonListener(PauseState::GetInstance());
			InputManager::Instance()->RemoveButtonListener(this);
			InputManager::Instance()->RemoveMousePosListener(this);
			InputManager::Instance()->ShowMouseCursor();
		}
		else 
			// else do the alpha calculation to fade out
			alpha += (float)(dt * 1.5);
	}

	if(m_gameover){
		// if it is fully blended in change state
		if(gameOverAlpha >= 1.0f){
			HighScoreState::GetInstance()->SetCurrentScore(m_score);
			mGameStateManager->ChangeState(HighScoreState::GetInstance());
			//Pop the current Context and add the new context for the input
			InputManager::Instance()->PopContext();
			InputManager::Instance()->PushContext(L"menucontext");
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->AddMenuButtonListener(HighScoreState::GetInstance());
			InputManager::Instance()->RemoveButtonListener(this);
			InputManager::Instance()->RemoveMousePosListener(this);
			InputManager::Instance()->ShowMouseCursor();
		}
		else 
			// else do the alpha calculation to fade out
			gameOverAlpha += (float)(dt * 1.5);
	}
	
	// If the game is not over update all the game components 
	if(!m_gameover){

		// Update the spaceship orientation
		pSpaceship->orient(heading, pitch, roll);

		// Update all the objects in the game world
		GameWorld::GetInstance()->Update(dt);
		// Update all the Particle Effects
		ParticleManager::GetInstance()->Update(dt);
		// Update the Star field particle effect
		pStarField->SetMatrix(pSpaceship->GetWorldMatrix());
		pStarField->Update(dt);
		// Constantly update the radar
		rad.SetPos(pSpaceship->GetPosition());
		rad.SetPlayerLookAt(pSpaceship->GetForwardVector());
		rad.Update(dt);
		// Update the Player and Decal Managers
		mPlayerManager.Update();
		mDecalManager.Update();

		// This makes sure when energy is depleted that sprint effect is not working
		// NOTE: without this sprint will be active even when energy is depleted UNTIL the button is released
		// With this now sprint will stop working immediatly when energy is 0
		if(m_energyDepleted){
			sprinting = false;
			pStarField->SetSprint(false);
			pSpaceship->SetSprint(false);
		}
		// Send all the listeners the elapsed time since this game started
		mGameTime.FireElapsedTime();

		// Reset the values 
		roll = 0.0f;
		pitch = 0.0f;
	}
}

void GameplayState::Render()
{
	// Clear the screen
	Graphics::GetInstance()->ClearScreen();
	// Power Up the Light and choose the types of light required
	mLight.PowerUpLight();
	mLight.SwitchOnLight(GL_LIGHT1);
	// Reset the Matrix
	Graphics::GetInstance()->ResetMatrix();
	// Set the new view matrix 
	Graphics::GetInstance()->CallModelViewMatrix();
	Graphics::GetInstance()->LoadMatrix(pSpaceship->GetCamera()->GetViewMatrix());
	// Render the Light
	mLight.Render();
	// Render the Skybox
	pSkybox->Render(pSpaceship->GetCamera()->GetPosition().x , pSpaceship->GetCamera()->GetPosition().y , pSpaceship->GetCamera()->GetPosition().z, 5000, 5000, 5000);
	// Render the Game World with all its objects
	GameWorld::GetInstance()->Render();
	// Render the Particles currently active
	ParticleManager::GetInstance()->Render();
	// Render the Radar
	rad.Render();
	// Render the starfield particle effect when it is active
	pStarField->Render();
	// Render the Player HUD on screen
	mHUD->Render();
	// Render The Decal Manager for the screen effects - needs to be drawn last
	mDecalManager.Render();
	// Render the White Flash when game ends
	Graphics::GetInstance()->DisableLighting();
	Graphics::GetInstance()->Disable2DTextures();
	Graphics::GetInstance()->Blend_SRCWithOne_MinusSRCAlpha();
	Graphics::GetInstance()->DisableDepthTest();
	if(m_gameover) {
		flash.SetColor(1, 1, 1, gameOverAlpha);
		flash.Render();
	}
	Graphics::GetInstance()->Enable2DTextures();
	Graphics::GetInstance()->DisableBlend();
	Graphics::GetInstance()->AllowDepthTest();

	/////////////OUTPUT THE FPS and objects rendered//////////////// its only here for testing
	std::stringstream ss;
	ss << " STAR RAGE ";
	//ss <<" | Test - " << pEnemy->GetTest();//mWorld.GetOctree()->GetCount();//m_FPS;
	ss <<" | Objects - " << GameWorld::GetInstance()->GetObjectsRendered();
	if(m_bUnkillable)
		ss << " | Invincible ";

	#ifdef _WIN32
	GameWindow::GetInstance()->SetWindowCaption(ss.str());
	#else
	GameWindow::GetInstance()->SetWindowCaption(ss.str());
	#endif
}

// Load all the asteroids
void GameplayState::LoadAsteroids()
{
	// for ever asteroid
	for(int i = 0; i < MAX_ASTEROIDS; i++){
		// if a asteroid is NULL then initialise it and set half of the asteroids on the left
		// and the other half on the right side
		if(pAsteroid[i].GetPtr() == NULL){
			if(i > (MAX_ASTEROIDS/2))
				pAsteroid[i] = new Asteroid(Asteroid::LEFT);
			else
				pAsteroid[i] = new Asteroid(Asteroid::RIGHT);
		}
		// reset all the asteroids
		pAsteroid[i]->Reset();
		// Generate a random position
		int range = 200;
		float x = float(rand() % range) - float(range) /2.0f;
		float y = float(rand() % range) - float(range) /2.0f;
		float z = float(rand() % range) - float(range) /2.0f; 
		pAsteroid[i]->SetPosition(Vec3(x, y, z));
		pAsteroid[i]->SetScale(0.5f);
		GameWorld::GetInstance()->AddGameObject(pAsteroid[i]);
		GameWorld::GetInstance()->AddCollisionEntity(pAsteroid[i]);
		ClassManager::GetInstance()->RegisterClassA(pAsteroid[i]);
		mGameTime.AddListener(pAsteroid[i]);
	}
}