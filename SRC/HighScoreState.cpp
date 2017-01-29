/*
This is the HighScore Table that will show 5 top highscores reached in this game. Currently to read
character input I am using the Windows messages instead of the DirectInput messages as it is easier to 
conver the windows messages straight into characters.

This High score table is called upon when the game is over showing the current score at the bottom. 
If it is a new highscore then it Will ask to enter the name so that it can be added to the HighScore list.
If there are more than n number of objects in the list it will remove the lowest one to make way for the
current new high score.

Author : Romesh Selvanathan

Last Edited: 29/02/2012

*/

#include "GameUtil.h"
#include "HighScoreState.h"
#include "MainMenuState.h"
#include "MessageBox.h"
#include "GameState.h"
#include "GameStateManager.h"
#include "MenuEntry.h"
#include "InputManager.h"
#include "Graphics.h"
#include "Texture.h"
#include "GL2DBox.h"
#include "PreLoader.h"
#include "GLText.h"
#include <algorithm>
#include "ClassManager.h"
#include "ScreenIDList.h"

HighScoreState* HighScoreState::mHighScoreState;

/* Constructor */
HighScoreState::HighScoreState() : GameState(ScreenIDList::GetInstance()->highscore)
{
	// Set current score to 0
	m_currentScore = 0;

	ClassManager::GetInstance()->RegisterScreen(this);
}

/* Destructor */
HighScoreState::~HighScoreState()
{
	// If the list is not empty destroy all objects in there
	if(!menuEntries.empty()){
		for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
			SAFE_DELETE((*it));
	}

	GameState::~GameState();
}

/* Initialise this state */
void HighScoreState::Init(GameStateManager* mGameState)
{
	// Initalise the buttons
	if(menuEntries.empty()){
		menuEntries.push_back(new MenuEntry("Menu", PreLoader::GetInstance()->menuButton->GetTGATexture().texID,
			WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT - 900));
	}
	m_EnterName = false;

	mInactive = mMessageActive = false;
	alpha = 0.0f;
	mJustSwitched = true;
	switchAlpha = 1.0f;
		
	mGameStateManager = mGameState;

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	// Setup the background box
	background.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	background.SetTexture(PreLoader::GetInstance()->background->GetTGATexture().texID);

	// Create Title Texture and set the Two Title squares up
	title.Init(500, 200, WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT - 200);
	title.SetTexture(PreLoader::GetInstance()->highScore_title->GetTGATexture().texID);
	titleGlow.Init(500, 200, WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT - 200);
	titleGlow.SetTexture(PreLoader::GetInstance()->highScore_title->GetTGATexture().texID);
	mGlow = 0;
	cosAdd = 0;

	// Create Both the Fonts which will be used to show the new Name that will be added into the 
	// HighScore table and the current score that will be shows even if it is not a high score
	if(m_newNameText.GetPtr() == NULL)
		m_newNameText = new GLText();
	m_newNameText->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	m_newNameText->SetColor3f(1, 1, 0);
	m_newNameText->SetPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT - 800);

	if(m_currentScoreText.GetPtr() == NULL)
		m_currentScoreText = new GLText();
	m_currentScoreText->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	m_currentScoreText->SetColor3f(1, 1, 0);
	m_currentScoreText->SetPosition(WINDOW_WIDTH/2 - 125, WINDOW_HEIGHT - 750);

	// Clear the list of scores
	scores.clear();
	// Read in the values from the file and store the score object
	ifstream inputFile("Resources//highscore.txt",ios::in);
	while(!inputFile.eof() && !inputFile.fail())
	{
		Score s;
		inputFile >> s.name >> s.score;
		s.text = new GLText();
		s.text->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
		scores.push_back(s);
	}
	// Sort the scores
	sort(scores.begin(), scores.end());
	// temp value to hold the highest score in the list
	int highestScoreinFile = 0;
	// if list is not empty obtain the highest score
	if(scores.size() > 0)
		highestScoreinFile = scores[0].score;
	// if current score is not 0 and the currentscore is higher than the highest score than a new
	// high score has to be entered
	if(m_currentScore > 0 && m_currentScore > highestScoreinFile)
		m_EnterName = true;
}

void HighScoreState::Resume()
{
	mMessageActive = false;
}

////////////////////////////////////IKeyCharListener////////////////////////////////////////////
void HighScoreState::OnCharKeyDown(WPARAM param)
{
	// If a new highscore needs to be entered
	if(m_EnterName) {
		// If backspace and if the string is not empty remove the last entered character
		if(param == 0x08){
			if(m_NameArray.size() > 0)
				m_NameArray.pop_back();
		}
		// Ignore keys which are not a displayable characters for example tab and backspace
		if(param > 32) {
			char k = (TCHAR) param;
			if(m_NameArray.size() < 20)
				m_NameArray.push_back(k);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

void HighScoreState::Update(float dt)
{
	if(mJustSwitched){
		if(switchAlpha < 0)
			mJustSwitched = false;
		switchAlpha -= 0.015f * 2;
	}

	if(mInactive){
		// if it is fully blended in change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(MainMenuState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(MainMenuState::GetInstance());
		}
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	}

	// Update all the buttons - the glow effect when hovering over them
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Update();

	// the value 0.0xx controls how quickly the glow occurs - higher the number the more times it flashes brightly
	cosAdd += 0.081;
	// using the cosing graph to obtain value between 0 and 1 to create the pulsating glow effect
	mGlow = (float)cos(cosAdd);
}

void HighScoreState::Render()
{
	Graphics::GetInstance()->ClearScreen();

	// Draw background
	background.Render();

	// If a new highscore is available show the entered name on screen
	if(m_EnterName)
		m_newNameText->print("Enter Name : %s", m_NameArray.c_str());
	// Show the current score regardless of whether its a highscore or not
	m_currentScoreText->print("Score: %d", m_currentScore);

	// The count value makes sure the position of each element is uniformaly spread downwards
	int count = 7;
	// Start from the back as the highscores are stored backwards so the best score is at the end of the list
	// NOTE: its score.end() - 1 as it seems to print a extra value right at the end even though only 5 are present in the 
	// text file
	for(int i = 0; i < scores.size(); i ++){
		scores[i].text->SetColor3f(1, 1, 1);
		scores[i].text->SetPosition(WINDOW_WIDTH/2 - 300, WINDOW_HEIGHT - (50 *count));
		scores[i].text->print("%20s - %i", scores[i].name.c_str(), scores[i].score);
		count ++;
	}

	// Render Each button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Render();

	// Allow additive blending GL_ONE to dest and SRC blending using alpha channel
	Graphics::GetInstance()->DisableDepthTest();
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	// Render the title and the title glow effect behind the title to create the flashing glow
	title.Render();
	titleGlow.SetColor(1, 1, 1, mGlow);
	titleGlow.Render();
	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();

	// Render the two Fade in and out effects when they are ready
	Graphics::GetInstance()->DisableLighting();
	Graphics::GetInstance()->Disable2DTextures();
	Graphics::GetInstance()->Blend_SRCWithOne_MinusSRCAlpha();
	Graphics::GetInstance()->DisableDepthTest();
	if(mInactive) {
		flash.SetColor(1, 1, 1, alpha);
		flash.Render();
	}

	if(mJustSwitched) {
		flash.SetColor(1, 1, 1, switchAlpha);
		flash.Render();
	}
	Graphics::GetInstance()->Enable2DTextures();
	Graphics::GetInstance()->DisableBlend();
	Graphics::GetInstance()->AllowDepthTest();
	
}

void HighScoreState::Selected()
{
	// Go thorught each button in the list and see if it has been clicked and depending on the button
	// perform the desired effect
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++){
		if((*it)->IsClicked() && (*it)->GetName() == "Menu"){
			mInactive = true;
			return;
		}
	}
}

/////////////////////////////////////////////////////private/////////////////////////////////////////////////////

void HighScoreState::AddNewScore(const string& name, int score)
{
	// Create a temp score object
	Score newScore;
	newScore.name = name;
	newScore.score = score;
	newScore.text = new GLText();
	newScore.text->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	// Add it to the current score list
	scores.push_back(newScore);
	// Sort the list again
	sort(scores.begin(), scores.end());

	// If there are too many scores remove last one
	while(scores.size() > 5)
		scores.pop_back();
	// Deactivate the highscore flag
	m_EnterName = false;
	// Save the score in a file
	SaveScore();
}

void HighScoreState::SaveScore()
{
	// Open up the highscore file and enter all the names and scores into the file
	ofstream outputFile("Resources//highscore.txt");
	if(outputFile.fail())
		return;

	// Write all the entries into the file
	for(ScoreList::iterator it = scores.begin(); it != scores.end(); it++)
	{
		outputFile << it->name << " " << it->score << "\n";
	}
}

//---------------------------------------IMenuButtonListener-------------------------------------
void HighScoreState ::OnMouseClick()
{
	Selected();
}

void HighScoreState ::OnUpPressed() {return;}
void HighScoreState ::OnDownPressed() {return;}
void HighScoreState ::OnLeftPressed() {return;}
void HighScoreState ::OnRightPressed() {return;}

void HighScoreState ::OnSelectPressed() 
{
	AddNewScore(m_NameArray, m_currentScore);
}
void HighScoreState::OnMouseMove(float x, float y)
{
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->UpdateMousePosition(x, y);
}
//-----------------------------------------------------------------------------------------------


