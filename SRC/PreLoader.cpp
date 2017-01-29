/*
This class just loads all the textures to be used in game. This class makes sure that the textures
are pre loaded only once and can be used in game multiple times. This saves memory and makes the game runs faster

AUthor : Romesh Selvanathan
*/

#include "GameUtil.h"
#include "PreLoader.h"
#include "Model_3DS.h"
#include "GLTexture.h"

PreLoader* PreLoader::mInstance;

PreLoader::PreLoader()
{
	//------------------------------------------------------Textures--------------------------------------------

	particle = new Texture();
	particle->LoadTGA("Textures//Effects//Particle.tga", Texture::MIPMAPPED);
	smoke = new Texture();
	smoke->LoadTGA("Textures//Effects//smoke.tga", Texture::MIPMAPPED);
	fire = new Texture();
	fire->LoadTGA("Textures//Effects//fire.tga", Texture::MIPMAPPED);
	background = new Texture();
	background->LoadTGA("Textures//Menu//Menu_BackGround.tga", Texture::MIPMAPPED);
	help_background = new Texture();
	help_background->LoadTGA("Textures//Menu//Menu_Help.tga", Texture::MIPMAPPED);
	messageBox = new Texture();
	messageBox->LoadTGA("Textures//Menu//Menu_MessageBox.tga", Texture::MIPMAPPED);
	messageBox_keybind = new Texture();
	messageBox_keybind->LoadTGA("Textures//Menu//Menu_MessageBox_Keybind.tga", Texture::MIPMAPPED);
	messageBox_save = new Texture();
	messageBox_save->LoadTGA("Textures//Menu//Menu_MessageBox_Save.tga", Texture::MIPMAPPED);
	pauseBox = new Texture();
	pauseBox->LoadTGA("Textures//Menu//Menu_Pause.tga", Texture::MIPMAPPED);
	messageBox_mask = new Texture();
	messageBox_mask->LoadTGA("Textures//Menu//Menu_MessageBox_Mask.tga", Texture::MIPMAPPED);
	keybinds_background = new Texture();
	keybinds_background->LoadTGA("Textures//Menu//Menu_Keybinds.tga", Texture::MIPMAPPED);
	keybinds_background_mask = new Texture();
	keybinds_background_mask->LoadTGA("Textures//Menu//Menu_Keybinds_Mask.tga", Texture::MIPMAPPED);
	tickbox_unticked = new Texture();
	tickbox_unticked->LoadTGA("Textures//Menu//Menu_TickBox_Unticked.tga", Texture::MIPMAPPED);
	tickbox_ticked = new Texture();
	tickbox_ticked->LoadTGA("Textures//Menu//Menu_TickBox_Ticked.tga", Texture::MIPMAPPED);
	tickbox_mask = new Texture();
	tickbox_mask->LoadTGA("Textures//Menu//Menu_TickBox_Mask.tga", Texture::MIPMAPPED);
	scroll_background = new Texture();
	scroll_background->LoadTGA("Textures//Menu//Scroll//Menu_Scroll_Background.tga", Texture::MIPMAPPED);
	scroll_background_left = new Texture();
	scroll_background_left->LoadTGA("Textures//Menu//Scroll//Menu_Scroll_Background_Left.tga", Texture::MIPMAPPED);
	scroll_background_right = new Texture();
	scroll_background_right->LoadTGA("Textures//Menu//Scroll//Menu_Scroll_Background_Right.tga", Texture::MIPMAPPED);
	scroll_orb = new Texture();
	scroll_orb->LoadTGA("Textures//Menu//Scroll//Menu_Scroll_Orb.tga", Texture::MIPMAPPED);
	scroll_orb_mask = new Texture();
	scroll_orb_mask->LoadTGA("Textures//Menu//Scroll//Menu_Scroll_Orb_Mask.tga", Texture::MIPMAPPED);
	title = new Texture();
	title->LoadTGA("Textures//Menu//Menu_Title.tga", Texture::MIPMAPPED);
	highScore_title = new Texture();
	highScore_title->LoadTGA("Textures//Menu//HighScore_Title.tga", Texture::MIPMAPPED);
	introButton = new Texture();
	introButton->LoadTGA("Textures//Menu//Menu_Button2.tga", Texture::MIPMAPPED);
	startButton = new Texture();
	startButton->LoadTGA("Textures//Menu//Menu_Button_Start.tga", Texture::MIPMAPPED);
	quitButton = new Texture();
	quitButton->LoadTGA("Textures//Menu//Menu_Button_Quit.tga", Texture::MIPMAPPED);
	scoreButton = new Texture();
	scoreButton->LoadTGA("Textures//Menu//Menu_Button_Score.tga", Texture::MIPMAPPED);
	okButton = new Texture();
	okButton->LoadTGA("Textures//Menu//Menu_Button_Ok.tga", Texture::MIPMAPPED);
	cancelButton = new Texture();
	cancelButton->LoadTGA("Textures//Menu//Menu_Button_Cancel.tga", Texture::MIPMAPPED);
	resumeButton = new Texture();
	resumeButton->LoadTGA("Textures//Menu//Menu_Button_Resume.tga", Texture::MIPMAPPED);
	menuButton = new Texture();
	menuButton->LoadTGA("Textures//Menu//Menu_Button_Menu.tga", Texture::MIPMAPPED);
	helpButton = new Texture();
	helpButton->LoadTGA("Textures//Menu//Menu_Button_Help.tga", Texture::MIPMAPPED);
	mouseButton = new Texture();
	mouseButton->LoadTGA("Textures//Menu//Menu_Button_Mouse.tga", Texture::MIPMAPPED);
	keybindsButton = new Texture();
	keybindsButton->LoadTGA("Textures//Menu//Menu_Button_Keybinds.tga", Texture::MIPMAPPED);
	optionsButton = new Texture();
	optionsButton->LoadTGA("Textures//Menu//Menu_Button_Options.tga", Texture::MIPMAPPED);
	defaultButton = new Texture();
	defaultButton->LoadTGA("Textures//Menu//Menu_Button_Default.tga", Texture::MIPMAPPED);
	saveButton = new Texture();
	saveButton->LoadTGA("Textures//Menu//Menu_Button_Save.tga", Texture::MIPMAPPED);
	buttonMask = new Texture();
	buttonMask->LoadTGA("Textures//Menu//Menu_Button_Mask.tga", Texture::MIPMAPPED);
	laser = new Texture();
	laser->LoadTGA("Textures//Laser.tga", Texture::MIPMAPPED);
	laser_enemy = new Texture();
	laser_enemy->LoadTGA("Textures//Laser_Enemy.tga", Texture::MIPMAPPED);
	radarTarget = new Texture();
	radarTarget->LoadTGA("Textures//Radar//target.tga", Texture::MIPMAPPED);
	radarOutofView = new Texture();
	radarOutofView->LoadTGA("Textures//Radar//OutOfView.tga", Texture::MIPMAPPED);
	radarOutofView_Enemy = new Texture();
	radarOutofView_Enemy->LoadTGA("Textures//Radar//OutOfView_Enemy.tga", Texture::MIPMAPPED);
	radarOutofView_Power = new Texture();
	radarOutofView_Power->LoadTGA("Textures//Radar//OutOfView_Power.tga", Texture::MIPMAPPED);
	radarOutofView_HardEnemy = new Texture();
	radarOutofView_HardEnemy->LoadTGA("Textures//Radar//OutOfView_HardEnemy.tga", Texture::MIPMAPPED);
	powerup_energy = new Texture();
	powerup_energy->LoadTGA("Textures//PowerUp//PowerUp_Energy.tga", Texture::MIPMAPPED);
	powerup_health = new Texture();
	powerup_health->LoadTGA("Textures//PowerUp//PowerUp_Health.tga", Texture::MIPMAPPED);
	player_booster = new Texture();
	player_booster->LoadTGA("Textures//Player_Spaceship_Booster.tga", Texture::MIPMAPPED);
	hud_aim_texture = new Texture();
	hud_aim_texture->LoadTGA("Textures//aim.tga", Texture::MIPMAPPED);
	hud_HealthHUD_tex = new Texture();
	hud_HealthHUD_tex->LoadTGA("Textures//HealthBar//HealthHUD.tga", Texture::MIPMAPPED);
	hud_testMask = new Texture();
	hud_testMask->LoadTGA("Textures//HealthBar//HealthHUD_Mask.tga", Texture::MIPMAPPED);
	hud_Health_tex = new Texture();
	hud_Health_tex->LoadTGA("Textures//HealthBar//HealthHUD_Health.tga", Texture::MIPMAPPED);
	hud_Energy_tex = new Texture();
	hud_Energy_tex->LoadTGA("Textures//HealthBar//HealthHUD_Energy.tga", Texture::MIPMAPPED);
	decal_glass = new Texture();
	decal_glass->LoadTGA("Textures//Decals//BrokenGlass.tga", Texture::MIPMAPPED);
	decal_blood = new Texture();
	decal_blood->LoadTGA("Textures//Decals//BloodSplatter.tga", Texture::MIPMAPPED);
}