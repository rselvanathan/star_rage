#include "GameState.h"

/* Renders The Fadeout effect */ 
void GameState::Render()
{
	if(mInactive)
		FadeOut();
}

void GameState::Render2()
{
	glPushMatrix();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

		glTranslatef(0.0f, 0.0f, -7.0f);
		glColor4f(1, 1, 1, alpha * 3);
		glBegin(GL_QUADS);
		  glVertex3f(-4.5f, 3.5f, 0.0f);              
		  glVertex3f( 4.5f, 3.5f, 0.0f);              
          glVertex3f( 4.5f,-3.5f, 0.0f);             
          glVertex3f(-4.5f,-3.5f, 0.0f);
		glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE,GL_ONE);

	glPopMatrix();
}

/* Creates a rectangular box which will have an alpha value
   creating the illusion of an fade effect as the alpha value is increased
   in the children's Update function
*/
void GameState::FadeOut()
{
	glPushMatrix();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

		glTranslatef(0.0f, 0.0f, -7.0f);
		glColor4f(0.0f, 0.0f, 0.0f, alpha);
		glBegin(GL_QUADS);
		  glVertex3f(-4.5f, 3.5f, 0.0f);              
		  glVertex3f( 4.5f, 3.5f, 0.0f);              
          glVertex3f( 4.5f,-3.5f, 0.0f);             
          glVertex3f(-4.5f,-3.5f, 0.0f);
		glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE,GL_ONE);

	glPopMatrix();
}