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

References - http://www.gamedev.net/topic/491246-get-text-typed-on-wm_keydown/
		   - http://msdn.microsoft.com/en-us/library/windows/desktop/ms646268(v=vs.85).aspx
*/

#ifndef __HIGHSCORESTATE_H__
#define __HIGHSCORESTATE_H__

#include "GameUtil.h"
#include "GameState.h"
#include "GL2DBox.h"
#include "IKeyCharListener.h"
#include "IMenuButtonListener.h"

class MenuEntry;
class InputManager;
class Texture;
class GLText;

// Struct which will hold name score and the Pointer to a font which will be used to
// print onto the screen
struct Score{
	string name;
	int score;
	SmartPtr<GLText> text;

	// Overload < function to be used in std::sort
	// I am overloading < into > as this makes sure that objects
	// are ordered in the right order instead of the opposite where
	// highest object will be placed in the back of the list.
	bool operator<(const Score &t){
		if(this->score > t.score)
			return true;
		return false;
	}
};

class HighScoreState : public GameState, public IKeyCharListener, public IMenuButtonListener
{
public :
	static HighScoreState* GetInstance()
	{
		if(mHighScoreState == NULL)
			mHighScoreState = new HighScoreState();
		return mHighScoreState;
	} 

	virtual ~HighScoreState();

	void Init(GameStateManager* mGameState);
	void Exit(){}
	void Pause(){}
	void Resume();

	void Update(float dt);
	virtual void Render();
	bool HandleMessage(const Telegram& msg){return false;}

	//////////////////////////////////////////IKeyCharListener/////////////////////////////////
	// Listens for any character key presses directly from windows events - used for name entering
	virtual void OnCharKeyDown(WPARAM param);
	///////////////////////////////////////////////////////////////////////////////////////////

	//---------------------------------------IMenuButtonListener-------------------------------------
	void OnMouseClick();
	void OnUpPressed();
	void OnDownPressed();
	void OnLeftPressed();
	void OnRightPressed();
	void OnSelectPressed();
	void OnMouseMove(float x, float y);
	//-----------------------------------------------------------------------------------------------

	void SetCurrentScore(int score) {m_currentScore = score;}

private:
	int m_currentScore;					// Current score of player
	bool m_EnterName;					// Does the player need to enter his/her name?
	string m_NameArray;					// The current name being entered
	SmartPtr<GLText> m_newNameText;		// THe new highscore name to be displayed when entering the name
	SmartPtr<GLText> m_currentScoreText; // The current score

	void SaveScore();					// Save the current high score
	void AddNewScore(const string& name, int score);	// Add a new score in the high-score table and insert it in the right place

	typedef vector<SmartPtr<MenuEntry> > MenuEntryList;
	MenuEntryList menuEntries;

	typedef vector<Score> ScoreList;	// List of Scores
	ScoreList scores;

	static HighScoreState* mHighScoreState;
	HighScoreState();

	void Selected();					// Checks Which button has been pressed and allows the desired effect

	GL2DBox background;					// The background image for this screen
	GL2DBox title;						// The Title for the screen
	GL2DBox titleGlow;					// The Title Glow effect for the screen
	float mGlow;						// Value used to allo wglow effect for the title
	float cosAdd;						// Value used to allo wglow effect for the title

	GL2DBox flash;						// THe White flash screen if switching from one screen to another
};

#endif