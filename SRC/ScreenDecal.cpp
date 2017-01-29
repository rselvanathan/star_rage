/*
This is a generic class that will generate a Screen Decal effect, i.e Blood Splatter. This class
requires the texture, time till it dies and the initial alpha value set for each decall type.
The user simply has to set the three values and update the current time value, in order for the decal
to dissappear.

Author : Romesh Selvanathan
Date : 03/05/12
*/

#include "ScreenDecal.h"
#include "GL2DBox.h"
#include "FrameTimer.h"
#include "GameUtil.h"

// Constructor, that requires texture, time till it dissapears, and the initial alpha
ScreenDecal::ScreenDecal(unsigned int texture, float timeTillDead, float alpha)
{
	// Set up the Quad to apply the decal texture onto
	m_decalQuad.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	m_decalQuad.SetTexture(texture);

	m_fCurrentTime = 0;
	m_fCountdown = 0;
	m_fTimeTillDead = timeTillDead;
	m_fAlpha = alpha;
	m_bActive = false;
}

// Constructor, that requires time till it dissapears, and the initial alpha
ScreenDecal::ScreenDecal(float timeTillDead, float alpha)
{
	// Set up the Quad to apply the decal texture onto
	m_decalQuad.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);

	m_fCurrentTime = 0;
	m_fCountdown = 0;
	m_fTimeTillDead = timeTillDead;
	m_fAlpha = alpha;
	m_bActive = false;
}

// Reset the Decal
void ScreenDecal::Reset()
{
	m_fCurrentTime = 0;
	m_fCountdown = 0;
	m_bActive = false;
}

// Update the decal
void ScreenDecal::Update()
{
	// If it is active 
	if(m_bActive) {
		// alpha is the initial alpha minus the value between 0 and 1 that will be obtained
		// by (currentTime - time when active was called) / the time specified for it to stay on
		float alpha = m_fAlpha - ((m_fCurrentTime - m_fCountdown)/m_fTimeTillDead);
		// if alpha reaches 0 then switch off the decal
		if(alpha <= 0)
			m_bActive = false;
		// Set the alpha
		m_decalQuad.SetColor(1, 1, 1, alpha);
	}
} 

// Render the decal
void ScreenDecal::Render()
{
	// If the decal is acive , Set Additive Blending and Render the Decal
	if(m_bActive) {
		Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
			m_decalQuad.Render();
		Graphics::GetInstance()->DisableBlend();
	}
}