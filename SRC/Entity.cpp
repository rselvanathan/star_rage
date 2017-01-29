/*
A Base Class for all Game Objects that are static

Author : Romesh Selvanathan
*/
#include "Entity.h"
#include "GameUtil.h"
#include "Model_3DS.h"

// Set the new world matrix
void Entity::SetWorldMatrix(const Matrix4 &worldMatrix)
{
	m_worldMatrix = worldMatrix;
	// obtain the orientation through the matrix (1st three columns)
    m_orientation.GetMatrix(worldMatrix);
	// Obtain the position of the entity
    mPosition = Vec3(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]);
	// Extract the three axes right, up and forward from the orientation quaternion
	m_orientation.ExtractOrientationAxis(mRight, mUp, mForward);
}

// Store the Model View Matrix
void Entity::PreRender()
{
	Graphics::GetInstance()->StoreMatrix();
}

// Render the object. If it has a 3D model associated with it then
// render the 3DS model otherwise use Graphics functions to render the entity
void Entity::Render()
{
	if(mModel.GetPtr() != NULL)
	{
		mModel->rot.x = pitch;
		mModel->rot.y = yaw;
		mModel->rot.z = roll;

		mModel->pos.x = mPosition.x;
		mModel->pos.y = mPosition.y;
		mModel->pos.z = mPosition.z;

		mModel->color.x = mRed;
		mModel->color.y = mGreen;
		mModel->color.z = mBlue;

		mModel->scale = mScale;

		mModel->Draw();
	}
	else 
	{
		Graphics::GetInstance()->Translate(mPosition.x, mPosition.y, mPosition.z);
		Graphics::GetInstance()->RotateX(pitch);
		Graphics::GetInstance()->RotateY(yaw);
		Graphics::GetInstance()->RotateZ(roll); 
		Graphics::GetInstance()->Color3(mRed, mGreen, mBlue);
		Graphics::GetInstance()->Scale(mScale, mScale, mScale);
	}
}

// Release the model view matrix
void Entity::PostRender()
{
	Graphics::GetInstance()->ReleaseMatrix();
}