/*
This class just loads all the textures to be used in game. This class makes sure that the textures
are pre loaded only once and can be used in game multiple times. This saves memory and makes the game runs faster

Author : Romesh Selvanathan
*/
#ifndef __PRELOADER_H__
#define __PRELOADER_H__

#include "GameUtil.h"
class Texture;
class Model_3DS;

class PreLoader
{
public:
	// Create A instance of this Preloader class
	static PreLoader* GetInstance()
	{
		if(mInstance == NULL)
			mInstance = new PreLoader();
		return mInstance;
	}
	//------------------------------------------------------Textures--------------------------------------------

	// All the textures to be loaded are public and can be accessed by any class
	SmartPtr<Texture> particle;
	SmartPtr<Texture> smoke;
	SmartPtr<Texture> fire;

	SmartPtr<Texture> background;
	SmartPtr<Texture> messageBox;
	SmartPtr<Texture> messageBox_keybind;
	SmartPtr<Texture> messageBox_save;
	SmartPtr<Texture> pauseBox;
	SmartPtr<Texture> messageBox_mask;
	SmartPtr<Texture> title;
	SmartPtr<Texture> highScore_title;
	SmartPtr<Texture> help_background;

	SmartPtr<Texture> keybinds_background;
	SmartPtr<Texture> keybinds_background_mask;
	SmartPtr<Texture> tickbox_unticked;
	SmartPtr<Texture> tickbox_ticked;
	SmartPtr<Texture> tickbox_mask;
	SmartPtr<Texture> scroll_background;
	SmartPtr<Texture> scroll_background_left;
	SmartPtr<Texture> scroll_background_right;
	SmartPtr<Texture> scroll_orb;
	SmartPtr<Texture> scroll_orb_mask;

	SmartPtr<Texture> introButton;
	SmartPtr<Texture> helpButton;
	SmartPtr<Texture> startButton;
	SmartPtr<Texture> quitButton;
	SmartPtr<Texture> scoreButton;
	SmartPtr<Texture> okButton;
	SmartPtr<Texture> cancelButton;
	SmartPtr<Texture> resumeButton;
	SmartPtr<Texture> menuButton;
	SmartPtr<Texture> mouseButton;
	SmartPtr<Texture> keybindsButton;
	SmartPtr<Texture> optionsButton;
	SmartPtr<Texture> defaultButton;
	SmartPtr<Texture> saveButton;
	SmartPtr<Texture> buttonMask;

	SmartPtr<Texture> laser;
	SmartPtr<Texture> laser_enemy;

	SmartPtr<Texture> radarTarget;
	SmartPtr<Texture> radarOutofView;
	SmartPtr<Texture> radarOutofView_Enemy;
	SmartPtr<Texture> radarOutofView_Power;
	SmartPtr<Texture> radarOutofView_HardEnemy;

	SmartPtr<Texture> powerup_energy;
	SmartPtr<Texture> powerup_health;

	SmartPtr<Texture> hud_aim_texture;
	SmartPtr<Texture> hud_HealthHUD_tex;
	SmartPtr<Texture> hud_testMask;
	SmartPtr<Texture> hud_Health_tex;
	SmartPtr<Texture> hud_Energy_tex;

	SmartPtr<Texture> player_booster;

	SmartPtr<Texture> decal_glass;
	SmartPtr<Texture> decal_blood;

private:
	PreLoader();
	static PreLoader* mInstance;
};

#endif