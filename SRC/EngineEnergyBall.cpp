/*
This object represents the engine energy ball that lights up behind the player spaceship, whenever the 
spaceship accelerates. It is similar achieved by placing 5 Quads in varying degrees facing the user.
Then simply blending the Quads with a texture a energy engine will be created. The velocity of the
player spaceship determines the alpha value of this object, making it dissapear or light up slowly when the 
player accelerates.

Author : Romesh Selvanathan

Date: 3/05/12
*/

#include "EngineEnergyBall.h"
#include "PreLoader.h"
#include "GameUtil.h"
#include "GLBox.h"
#include "Texture.h"

EngineEnergyBall::EngineEnergyBall(const char* mType, float width, float height, Vec3 position) : Entity(mType)
{
	if(mType == "spaceshipengine")
		texture = PreLoader::GetInstance()->player_booster->GetTGATexture().texID;

		Quad0.Init(width, height, 0);
		Quad0.SetTexture(texture);
		Quad20.Init(width,height, 0);
		Quad20.SetTexture(texture);
		Quad20.SetRotation(0, 20, 0);
		Quad40.Init(width, height, 0);
		Quad40.SetTexture(texture);
		Quad40.SetRotation(0, 40, 0);
		Quad60.Init(width, height, 0);
		Quad60.SetTexture(texture);
		Quad60.SetRotation(0, 60, 0);
		Quad80.Init(width, height, 0);
		Quad80.SetTexture(texture);
		Quad80.SetRotation(0, 80, 0);

	m_vRealtivePosition = position;
	mRadius = 0.5f;
}

EngineEnergyBall::~EngineEnergyBall()
{
	Entity::~Entity();
}

/* 
This will determine the alpha value of the Energy Ball as only when the player/enemy accelerate should it appear
and dissapear when the entity stops - in a smooth way
*/
void EngineEnergyBall::SetLife(Vec3 velocity)
{
	Quad0.SetColor(1, 1, 1, 1 * velocity.Length());
	Quad20.SetColor(1, 1, 1, 1 * velocity.Length());
	Quad40.SetColor(1, 1, 1, 1 * velocity.Length());
	Quad60.SetColor(1, 1, 1, 1 * velocity.Length());
	Quad80.SetColor(1, 1, 1, 1 * velocity.Length());
}

void EngineEnergyBall::Render()
{
	Graphics::GetInstance()->MultiplyToMatrix(m_worldMatrix);

	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	Graphics::GetInstance()->DisableDepthMask();
	Graphics::GetInstance()->DisableBackFaceCull();
	Graphics::GetInstance()->StoreMatrix();
		Quad0.SetPosition(m_vRealtivePosition.x, m_vRealtivePosition.y, m_vRealtivePosition.z);
		Quad0.Render();
		Quad20.SetPosition(m_vRealtivePosition.x, m_vRealtivePosition.y, m_vRealtivePosition.z);
		Quad20.Render();
		Quad40.SetPosition(m_vRealtivePosition.x, m_vRealtivePosition.y, m_vRealtivePosition.z);
		Quad40.Render();
		Quad60.SetPosition(m_vRealtivePosition.x, m_vRealtivePosition.y, m_vRealtivePosition.z);
		Quad60.Render();
		Quad80.SetPosition(m_vRealtivePosition.x, m_vRealtivePosition.y, m_vRealtivePosition.z);
		Quad80.Render();
	Graphics::GetInstance()->ReleaseMatrix();
	Graphics::GetInstance()->AllowBackFaceCull();
	Graphics::GetInstance()->AllowDepthMask();
	Graphics::GetInstance()->DisableBlend();
}
