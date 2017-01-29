/*
// LEGACY
Radar system that shows the objects on the map (so far only asteroids) - It takes the current position of the objects and then updates them relative
to the center of the radar with rotation - (Stil buggy as rotations is fine but then as game goes on starts to go wrong slightly ) NOT FINAL 

Reference : http://nfostergames.com/3DRadarSample.htm

Author : Romesh Selvanathan

Last Edit Date : 22/01/2012 = Added The Textures

// UPDATE: 16/02/2012
Complete change of radar. The whole screen has become the radar system, now what happens is same thing as before to test how far an object is from the spaceship 
and instead of showing it in a little radar, the objects positions are shown on the screen as target boxes or if they are not in view of the player as arrows
pointing where the player should look to see the objects in view - similar to this http://www.4gamer.net/patch/demo/freelancer.jpg
If the Object is an enemy the Arrows/Target Box will be shown regardless of distance
REFERENCES - http://www.gamedev.net/topic/327903-freelancer-style-radar/
		   - My own thread - http://www.gamedev.net/topic/620383-gluproject-odd-problem/

UPDATE 01/03/2012 - scaling of radar targets + powerups added
*/

#include "Radar.h"
#include "GameUtil.h"
#include "GL2DBox.h"
#include "Texture.h"
#include "GLBox.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Frustum.h"
#include "PreLoader.h"

const int POWER_RADAR_RANGE = 70;
const int ASTEROID_RADAR_RANGE = 50;

/* Constructor */
Radar::Radar()
{
	m_playerPos = m_playerLookAt = Vec3(0, 0, 0);

	test2 = 0;
}

/* Destructor */
Radar::~Radar()
{
	for(PointList::iterator it = points.begin(); it != points.end(); ++it){
		SAFE_DELETE((*it));
	}
}

/* Initialize */
void Radar::Init(float range)
{
}

void Radar::Reset()
{
	m_playerPos = m_playerLookAt = Vec3(0, 0, 0);
	test2 = 0;

	points.clear();
}

/* Update the Radar */
void Radar::Update(float dt)
{
	// Iterate through each point
	for(PointList::iterator it = points.begin(); it != points.end(); ++it)
	{
		// Obtain the Game Object Type
		GameObjectType object_type = (*it)->object->GetType();
		// Obtain the current position of the Object
		Vec3 temp = (*it)->object->GetPosition();
		// Work out the poisiton relative to the player position
		Vec3 direction = m_playerPos - temp;
		// Obtain the length of that vector
		float distance = direction.Length();

		// If its a Hard enemy then set the scale always high
		if(object_type == GameObjectType("hardenemy")) {
			// Set the point to be drawn
			(*it)->Draw = true;
			// Set up the box to be drawn on screen
			(*it)->Box.Init(50, 25, 0, 0);
			(*it)->Box.SetScale(1.5f);
		}
		// See if its an enemy then draw regardless of distance
		if(object_type == GameObjectType("bigEnemy") || object_type == GameObjectType("smallenemy")){
			// Set the point to be drawn
			(*it)->Draw = true;
			// Set up the box to be drawn on screen
			(*it)->Box.Init(50, 25, 0, 0);
			// Scale the box depending on the distance (multiplying by 3 and divided by 10 as these seem to give the best effect - trial and error)
			float scale = 10/distance * 3;
			// Cap the scale
			if (scale >= 2.0f) scale = 2.0f; if(scale <= 0.6f) scale = 0.6f;
			(*it)->Box.SetScale(scale);
		}
		if(object_type== GameObjectType("poweruphealth") || object_type == GameObjectType("powerupenergy"))
			if(distance < POWER_RADAR_RANGE) {
				// Set the point to be drawn
				(*it)->Draw = true;
				// Set up the box to be drawn on screen
				(*it)->Box.Init(50, 25, 0, 0);
				// Scale the box depending on the distance (multiplying by 3 and divided by 10 as these seem to give the best effect - trial and error)
				float scale = 10/distance * 3;
				// Cap the scale
				if (scale >= 2.0f) scale = 2.0f; if(scale <= 0.6f) scale = 0.6f;
				(*it)->Box.SetScale(scale);
			}
		if(object_type== GameObjectType("asteroid")) 
			if(distance < ASTEROID_RADAR_RANGE) {
				// Set the point to be drawn
				(*it)->Draw = true;
				// Set up the box to be drawn on screen
				(*it)->Box.Init(50, 25, 0, 0);
				// Not multiplying by 3 here as that would drstically increase and decrease the box size - the asteroid is not the main focus of the game
				// it should only show a big sign in the player is really close to the asteroid otherwise just a hint making sure player knows a asteroid is in his reach
				float scale = 10/distance;
				if (scale >= 2.0f) scale = 2.0f; if(scale <= 0.6f) scale = 0.6f;
				(*it)->Box.SetScale(scale);
			}
		// otherwise do not allow it to draw
		else {
			(*it)->Draw = false;
		}
	}
}

/* Render the Radar */
void Radar::Render()
{
	// Allow Blending (Additive Blending)
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	// Iterate through each object
	for(PointList::iterator it = points.begin(); it != points.end(); ++it){
		// If point is allowed to be drawn
		if((*it)->Draw) {
			// Store the Matrix();
			Graphics::GetInstance()->StoreMatrix();

			// Temporary values
			double modelview[16];
			double proj[16];
			int viewport[4];
			double winx, winy, winz;

			// Obtain current matrixes used by OpenGl
			Graphics::GetInstance()->GetProjectionMatrix(proj);
			Graphics::GetInstance()->GetModelViewMatrix(modelview);
			Graphics::GetInstance()->GetViewport(viewport);

			// This calculation below is necessary as it checks to see if the object is behind the view of the player
			// What happens with GluProject is that if checks wheter a object in 3D space intersets with the 2D screen space
			// So when it does it will convert the 3D position to 2D screen position. But the problem is when the camera
			// turns 180 degrees, the point in 2D screen will still appear due to the fact that the 3D point is stil intersecting with the 
			// view plane but the value will just be negative.
			// So in order to avoid allowing the point to be shown in both directions I calculate and see if the object is behind the
			// current players view. If it is then just switch the position of the point to make sure it does not appear when the 
			// player is not viewing it. 

			/* 
												 |
												 |
			Object in 3D	O----X---------------|--------------X------   the 3D intersection line
												 |
												 |
									The View Plane of Player (can think of as the screen)
									X - point which can be seen on screen

				Just a better way to visualise what is described above - the 3D intersection will occur on both sides even if Object is not in players view
				so the Point will appear when camera is turned 180 degrees from the object(not wanted) and when facing object(wanted)
			*/
			// Obtain the Object position
			GameObjectType object_type = (*it)->object->GetType();
			Vec3 temp = (*it)->object->GetPosition();
			// Work out the direction of object from player
			Vec3 direction = m_playerPos - temp;
			// Work out dot product from player to direction
			float dot = m_playerLookAt.Dot(direction);
			if(object_type == GameObjectType("smallenemy"))
			test2 = dot;
			// if GluProject succeeds
			//if(gluProject(temp.x, temp.y, temp.z, modelview, proj, viewport, &winx, &winy, &winz) == GL_TRUE)
			if(Graphics::GetInstance()->Project3DToScreen(temp, modelview, proj, viewport, winx, winy, winz))
			{
				// if Dot if less than a epsilon value (if value is below this than that means its behind the view of player)
				if(dot < 0.1f){
					// Change the Position of window x to - value so it does not show up on the wrong side
					winx *= -1;
				}

				// Set the Target Texture
				(*it)->Box.SetTexture(PreLoader::GetInstance()->radarTarget->GetTGATexture().texID);
				if(object_type == GameObjectType("bigEnemy") || object_type == GameObjectType("smallenemy") || object_type == GameObjectType("hardenemy"))
					(*it)->Box.SetColor(1, 0.2, 0.2, 1);
				else if (object_type == GameObjectType("poweruphealth") || object_type == GameObjectType("powerupenergy"))
					(*it)->Box.SetColor(0.8, 0.8, 0.2, 1);

				// If winX is greater than the maximum width then set the value to the maximum width and set the
				// out of view Texture instead
				if(winx > WINDOW_WIDTH- 25){
					winx = WINDOW_WIDTH - 25;
					if(object_type == GameObjectType("asteroid"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView->GetTGATexture().texID);
					else if (object_type == GameObjectType("poweruphealth") || object_type == GameObjectType("powerupenergy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Power->GetTGATexture().texID);
					else if (object_type == GameObjectType("hardenemy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_HardEnemy->GetTGATexture().texID);
					else
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Enemy->GetTGATexture().texID);
					(*it)->Box.SetRoll(-90);
					(*it)->Box.SetColor(1, 1, 1, 1);
				}
				// If winX is less than the minimum width then set the value to the minimum width and set the
				// out of view Texture instead
				if(winx < 20){
					winx = 25;
					if(object_type == GameObjectType("asteroid"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView->GetTGATexture().texID);
					else if (object_type == GameObjectType("poweruphealth") || object_type == GameObjectType("powerupenergy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Power->GetTGATexture().texID);
					else if (object_type == GameObjectType("hardenemy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_HardEnemy->GetTGATexture().texID);
					else
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Enemy->GetTGATexture().texID);
					(*it)->Box.SetRoll(90);
					(*it)->Box.SetColor(1, 1, 1, 1);
				 }
				// If winY is less than the maximum height then set the value to the maximum height and set the
				// out of view Texture instead
				if(winy > WINDOW_HEIGHT - 25){
					winy = WINDOW_HEIGHT - 25;
					if(object_type == GameObjectType("asteroid"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView->GetTGATexture().texID);
					else if (object_type == GameObjectType("poweruphealth") || object_type == GameObjectType("powerupenergy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Power->GetTGATexture().texID);
					else if (object_type == GameObjectType("hardenemy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_HardEnemy->GetTGATexture().texID);
					else
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Enemy->GetTGATexture().texID);
					(*it)->Box.SetRoll(0);
					(*it)->Box.SetColor(1, 1, 1, 1);
				}
				// If winY is less than the minimum height then set the value to the minimum height and set the
				// out of view Texture instead
				if(winy < 25){
					winy = 25;
					if(object_type == GameObjectType("asteroid"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView->GetTGATexture().texID);
					else if (object_type == GameObjectType("poweruphealth") || object_type == GameObjectType("powerupenergy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Power->GetTGATexture().texID);
					else if (object_type == GameObjectType("hardenemy"))
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_HardEnemy->GetTGATexture().texID);
					else
						(*it)->Box.SetTexture(PreLoader::GetInstance()->radarOutofView_Enemy->GetTGATexture().texID);
					(*it)->Box.SetRoll(180);
					(*it)->Box.SetColor(1, 1, 1, 1);
				 }

				// Set the Window Position obtained by GluProject and Render the 2D Box
				(*it)->Box.SetPosition(winx, winy);
				(*it)->Box.Render();
			}

			// Release the stored matrix
			Graphics::GetInstance()->ReleaseMatrix();
		}
	}
	
	/* Disable Blending */
	Graphics::GetInstance()->DisableBlend();

}