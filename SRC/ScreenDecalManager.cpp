/*
This class deals with the screen decals that will pop up in the game. It will listen to any messages and activate the appropriate decals,
and also update the time for each of the decals in available.

Author : Romesh Selvanathan
Date : 2/04/12
*/
#include "ScreenDecalManager.h"
#include "Decal_Blood.h"
#include "Decal_BrokenGlass.h"
#include "ClassIDList.h"
#include "MessageType.h"
#include "Telegram.h"

ScreenDecalManager::ScreenDecalManager() : BaseClass(ClassIDList::GetInstance()->class_ScreenDecalManager)
{
	// Initialse the Decal
	m_pBrokenGlass = new Decal_BrokenGlass(3000, 0.5);
	m_pBlood = new Decal_Blood(1000, 0.8);
}

ScreenDecalManager::~ScreenDecalManager()
{
	// Destroy the objects
	SAFE_DELETE(m_pBrokenGlass);
	SAFE_DELETE(m_pBlood);
}

void ScreenDecalManager::Init()
{
	// Reset the decals
	m_pBrokenGlass->Reset();
	m_pBlood->Reset();
}

bool ScreenDecalManager::HandleMessage(const Telegram& msg)
{
	// depending on the message enable the appropriate decal
	switch(msg.m_message)
	{
	case msg_decalBlood:
		{
			m_pBlood->SetActive();
		}
		return true;
	case msg_decalBrokenGlass:
		{
			m_pBrokenGlass->SetActive();
		}
		return true;
	}

	return false;
}

void ScreenDecalManager::Update()
{
	// Update all the decals
	m_pBlood->Update();
	m_pBrokenGlass->Update();
}

void ScreenDecalManager::Render()
{
	// Render all the decals
	m_pBrokenGlass->Render();
	m_pBlood->Render();
}

void ScreenDecalManager::OnTimer(float milliseconds)
{
	// Set the time for each decal
	m_pBlood->SetCurrentTime(milliseconds);
	m_pBrokenGlass->SetCurrentTime(milliseconds);
}