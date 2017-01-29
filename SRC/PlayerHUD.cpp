/* 
The PlayerHUD shows all the required HUD components for the player.

last Edit - 22.01.12 - Added Energy bar

Author : Romesh Selvanathan
*/

#include "PlayerHUD.h"
#include "GL2DBox.h"
#include "GameUtil.h"
#include "Texture.h"
#include "Graphics.h"
#include "GLText.h"
#include "PreLoader.h"
#include <cmath>

// Constructor
PlayerHUD::PlayerHUD()
{
	// Set up each 2D box
	aim.Init(100, 50, WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 + 200);
	aim.SetColor(1, 1, 1, 1);

	mHealthHud.Init(500, 200, -40, WINDOW_HEIGHT - 200);
	testMask.Init(500, 200, -40, WINDOW_HEIGHT - 200);

	mHealth.Init(312, 15, 55, WINDOW_HEIGHT - 143);
	mHealth.SetColor(1, 0, 0, 1);
	mEnergy.Init(312, 15, 55, WINDOW_HEIGHT - 218);
	mEnergy.SetColor(0, 0, 1, 1);

	// Set the textures to the appropriate boxes
	aim.SetTexture(PreLoader::GetInstance()->hud_aim_texture->GetTGATexture().texID);
	mHealthHud.SetTexture(PreLoader::GetInstance()->hud_HealthHUD_tex->GetTGATexture().texID);
	testMask.SetTexture(PreLoader::GetInstance()->hud_testMask->GetTGATexture().texID);

	// Set up the score text
	mScoreText = new GLText();
	mScoreText->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	mScoreText->SetPosition(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT - 100);
	mScoreText->SetColor3f(1, 1, 0);

	// The score itself
	m_score = 0;
}

PlayerHUD::~PlayerHUD()
{}

//////////////////////////////////////////////////////IPLAYERLISTENER//////////////////////////////////////////////////////
void PlayerHUD::OnHealthChanged(int health)
{
	// Set the Health bar depending on the health. Mutliply the health 
	// value with the width/100 (100 being total health) and then set that
	// value as health
	health = static_cast<float>(health);
	health *= 3.12f;
	mHealth.SetWidth(health);
}

void PlayerHUD::OnEnergyChanged(float energy)
{
	// Set the Energy bar depending on the energy. Mutliply the energy 
	// value with the width/100 (100 being total energy) and then set that
	// value as health
	energy *= 3.12f;
	mEnergy.SetWidth(energy);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////ISCORELISTENER//////////////////////////////////////////////////////
void PlayerHUD::OnScoreChanged(int score)
{
	// Set the score value
	m_score = score;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerHUD::Render()
{
	// Reset the ModelView Matrix
	Graphics::GetInstance()->ResetMatrix();
	// Render the score value
	mScoreText->print("Score - %i", m_score);
	// Disable Dept Tesing and allow Masking
	Graphics::GetInstance()->DisableDepthTest();
	Graphics::GetInstance()->Blend_Masking();
	// Render the mask first
	testMask.Render();
	// Render the normal picture
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	mHealthHud.Render();
	mHealth.Render();
	mEnergy.Render();
	aim.Render();
	// Disable blending and allow Depth test again
	Graphics::GetInstance()->DisableBlend();
	Graphics::GetInstance()->AllowDepthTest();
}
