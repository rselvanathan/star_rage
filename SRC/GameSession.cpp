/* 
This class contains the main loop and will set up the whole application itself. It is a singleton class

Author : Romesh Selvanathan

Date : 3/05/12
*/

#include "GameSession.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameStateManager.h"
#include "FrameTimer.h"
#include "GameplayState.h"
#include "HighScoreState.h"
#include "IntroState.h"
#include "Graphics.h"
#include "InputManager.h"
#include "GameWorld.h"

GameSession* GameSession::mInstance;

GameSession::~GameSession()
{
	SAFE_DELETE(mStateManager);
	SAFE_DELETE(mFps);
}

bool GameSession::Init(HINSTANCE hInstance, bool fullscreenflag)
{
	if(!GameWindow::GetInstance()->Init(hInstance, fullscreenflag))
		GameWindow::GetInstance()->KillGLWindow();

	// Create a new Frame Rate updater and set the constant frame rate
	mFps = new FrameTimer();
	mFps->init(75);

	// Initialise the Game State Manager
	mStateManager = new GameStateManager();
	// Preload the game to save loading time
	GameplayState::GetInstance();
	GameplayState::GetInstance()->Init(mStateManager);

	// Start off with the Intro State
	mStateManager->ChangeState(IntroState::GetInstance());
	InputManager::Instance()->AddMenuButtonListener(IntroState::GetInstance());

	// Set up the InputManager here and add the listeners
	InputManager::Instance()->Init(GameWindow::GetInstance()->GetHWND(), GameWindow::GetInstance()->GetHInstance());
	InputManager::Instance()->AddKeyListener(mStateManager.GetPtr());

	// Push the Menu Context first
	InputManager::Instance()->PushContext(L"menucontext");

	// Used to solve the issue with GameplayState reading input before game begins during Loading screen
	count = 0;

	return true;
}

void GameSession::Update()
{
	// Obtain the set number of frames
	int fps = mFps->framesToUpdate();
	/*float elapsedtime = mFps->GetElapsedTimeSinceLastUpdate();
	elapsedtime /= 1000;
	if(mStateManager)
		mStateManager->Update(elapsedtime);*/

	// temprary value
	int i = 0;
	// While value is less than frames per second simulate th world
	while( i < fps)
	{
		// Update every screen with a constant delta value of 0.01f
		if(mStateManager)
			mStateManager->Update(0.01f);
		 
		// increment the i value
		i ++;
	}
}

void GameSession::Render()
{
	// Render all the screens
	Graphics::GetInstance()->ClearScreen();
	if(mStateManager)
		mStateManager->Render();
}

WPARAM GameSession::MainLoop()
{
	MSG msg;

	while(1)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			// Update the mapper at every frame
			InputManager::Instance()->UpdateMapper();

			// Update and Render
			Update();
			Render();

			SwapBuffers(GameWindow::GetInstance()->GetHDC());				// Swap Buffers (Double Buffering)
        } 
	}

	GameWindow::GetInstance()->KillGLWindow();
	return(msg.wParam);									// Return from the progra
}
