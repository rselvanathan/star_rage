/* 
This class deals with the 3rd Person view of the world. The camera will avoid gimbal lock as it uses
Quaternions to obtain the final rotations. The camera does not use any calls to opengl to obtain the final
view matrix, so this class is flexible enough to be ported over to DirectX, requiring only minimal changes.
The class requires the target matrix and its rotations to follow an object, so it can be places behind 
any game object.

Author : Romesh Selvanathan
Date : 3/05/2012

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].

Fletcher, D and Parberry, I. (2002). 3D Math Primer for Graphics and Game Development. Texas: Wordware
*/

#include "Camera.h"
#include "GameUtil.h"
#include "Quaternion.h"
#include "Plane.h"

// Constructor
Camera::Camera() : mPosition(Vec3(0, 0, 0)),
				   mLookAt(Vec3(0, 0, 0)),
				   mUpVector(Vec3(0, 1, 0)),
				   m_xAxis(Vec3(1, 0, 0)),
				   m_yAxis(Vec3(0, 1, 0)),
				   m_zAxis(Vec3(0, 0, 1)),
				   m_viewDir(Vec3(0, 0, -1)),
				   m_offset(Vec3(0, 0, 0)),
				   m_headingDegrees(0.0f),
				   m_pitchDegrees(0.0f),
				   m_springConstant(16.0f),
				   m_dampingConstant(8.0f),
				   m_offsetDistance(0.0),
				   mVelocity(Vec3(0, 0, 0))
{
	m_viewMatrix.Identity();
	m_targetMatrix.Identity();
    m_orientation.Identity();

	m_sprint = false;
	fov = 55.0f;
	shake = 0.0f;
	gradualShake = 0.0f;
}

// Destructor
Camera::~Camera()
{}

// Reset the camera 
void Camera::Reset()
{
	mPosition = Vec3(0, 0, 0);
	mLookAt = Vec3(0, 0, 0);
	mUpVector = Vec3(0, 1, 0);
	m_xAxis = Vec3(1, 0, 0);
	m_yAxis = Vec3(0, 1, 0);
	m_zAxis = Vec3(0, 0, 1);
	m_viewDir= Vec3(0, 0, -1);
	m_offset= Vec3(0, 0, 0);
	m_headingDegrees = 0.0f;
	m_pitchDegrees= 0.0f;
	m_springConstant= 16.0f;
	m_dampingConstant= 8.0f;
	m_offsetDistance= 0.0;
	mVelocity= Vec3(0, 0, 0);

	m_viewMatrix.Identity();
	m_targetMatrix.Identity();
    m_orientation.Identity();

	m_sprint = false;
	fov = 55.0f;
}

/* Sets the Lookat Target for the camera */ 
void Camera::SetLookAt(const Vec3 &target)
{
	// changes it so that the camera is alwyas behind the target
	Vec3 temp = target;
	Vec3 newTarget = Vec3(temp.x, temp.y - 1.f, temp.z);
	mLookAt = target;
}

/*  
Sets the lookAt target for the camera in 3rd Person by specifying 
the position of camera, the target to look at and the up vector of the camera
You could say a custom version of gluLookAt. This method us usually called right 
at the beginnig (i.e. initalisation of the game)

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].
*/
void Camera::SetLookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up)
{
	// Obtain the value and set them to the current camera variables.
	mPosition = eye;
	mLookAt = target;
	mUpVector = up;

	// The offset vector is the vector from the target position to the camera
    // position. This happens to also be the local z axis of the camera.
    // The offset vector is always relative to the 'target' position.
	m_offset = m_zAxis = mPosition - mLookAt;
	m_zAxis.Normalize();
	// View Direction is the inversed vector of local Z axis which is relative to the target poition
	m_viewDir = m_zAxis.Inverse();
	
	// Obtain the X axis
	m_xAxis = m_viewDir.Cross(mUpVector);
    m_xAxis.Normalize();

	// Obtain the Y Axis
	m_yAxis = m_xAxis.Cross(m_viewDir);
    m_yAxis.Normalize();
    m_xAxis.Normalize();

	/* Set the view matrix of the camera */ 
    m_viewMatrix[0][0] = m_xAxis.x;
    m_viewMatrix[1][0] = m_xAxis.y;
    m_viewMatrix[2][0] = m_xAxis.z;
	m_viewMatrix[3][0] = -m_xAxis.Dot(mPosition);

    m_viewMatrix[0][1] = m_yAxis.x;
    m_viewMatrix[1][1] = m_yAxis.y;
    m_viewMatrix[2][1] = m_yAxis.z;
	m_viewMatrix[3][1] = -m_yAxis.Dot(mPosition);

    m_viewMatrix[0][2] = m_zAxis.x;
    m_viewMatrix[1][2] = m_zAxis.y;
    m_viewMatrix[2][2] = m_zAxis.z;    
	m_viewMatrix[3][2] = -m_zAxis.Dot(mPosition);

	// Turn the view matrix into a Quaternion
	m_orientation.GetMatrix(m_viewMatrix);
	// Obtain the offset distance 
	m_offsetDistance = m_offset.Length();
}

/*
Set the spring constant

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].
*/
void Camera::SetSpringConstant(float constant)
{
	// Using a critically damped spring system where damping ratio is = 1
	// damping ratio = damping constant / (2.0f*sqrtf(spring constant)) where in this case ratio = 1 
	m_springConstant = constant;
	m_dampingConstant = 2.0f*sqrtf(constant);
}

// Set the rotation values for the camera to rotate
void Camera::SetRotate(float longitudeDegrees, float latitudeDegrees, float z)
{
    // 'longitudeDegrees','latitudeDegrees' and 'm_zDegrees represents the maximum
    // number of degrees of rotation per second.

    m_pitchDegrees = -latitudeDegrees;
    m_headingDegrees = -longitudeDegrees;
	m_zDegrees = -z;
}

// Main Update body, which needs to be called at every frame update
void Camera::Update(float dt)
{
	Vec3 pos = mPosition;

	// If sprinting is enabled
	if (m_sprint)
	{
		// Allow camera to shake
		gradualShake += 0.002;
		if(gradualShake > 0.3f)
			gradualShake = 0.3f;
		shake += gradualShake;
		mLookAt.y += cos(shake) * 0.3;
		// Stop changing Field of view if you go beyond a certain value
		if(fov >= 75)
			fov = 75;
		else
			// add a certain amount to current fov
			fov += 0.5;
	}// if not sprinting
	else{
		// Stop shaking
		shake = 0.0f;
		gradualShake = 0.0f;
		// and if the value is greater than normal fov
		if(fov > 55.0f){
			// till you go below the normal fov
			if(fov <= 55.0f)
				fov = 55.0f;
			else// decrease the value
				fov -= 0.9;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// Calculate The Aspect Ratio Of The Window
														// The parameters are:
														// (view angle, aspect ration of the width to the height, 
														//  The closest distance to the camera before it clips, 
				  // FOV		// Ratio				//  The farthest distance before it stops drawing)
	gluPerspective(fov,(GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT,0.1f,7000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Update the Up Vector by obtaining the local up vector of the target. This is necessary, as otherwise
	// the camera will do a funny "switch" when it reaches its "world" up , not creating a smooth movement
	mUpVector = Vec3 (m_targetMatrix[1][0], m_targetMatrix[1][1], m_targetMatrix[1][2]);

	// Update the orientation and View Matrix
	UpdateOrientation(dt);
	UpdateViewMatrix(dt);
}

/*
Update the Orientation

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].
*/
void Camera::UpdateOrientation(float dt)
{
	// Rotation per second
	m_pitchDegrees *= (dt*100);
	m_headingDegrees *= (dt*100);
	m_zDegrees *= (dt*100);

	// Temporary Quaternion as well as the 3 local axes of the target
	Quaternion rotation = Quaternion::IDENTITY;
	Vec3 localXAxis(m_targetMatrix[0][0], m_targetMatrix[0][1], m_targetMatrix[0][2]);
    Vec3 localYAxis(m_targetMatrix[1][0], m_targetMatrix[1][1], m_targetMatrix[1][2]);
    Vec3 localZAxis(m_targetMatrix[2][0], m_targetMatrix[2][1], m_targetMatrix[2][2]);

	// if turning in the y axis
	if (m_headingDegrees != 0.0f)
    {
		// In order to allow free rotation in any direction have to use the local Y axis rather than a global one, where only rotations in one direction is allowed
		// so for ex if I roll 90 degrees and then want to rotate I am stil using the world Y axis rather than local, so it would seem tham I am looking up and down rather than
		// sideways. Se this below is the wrong one to use 
		//rotation.ConvertAxisToQuat(headingDegrees * DEG2RAD,Vec3(0.0f, 1.0f, 0.0f));

		// Convert into quaternion
		rotation.ConvertAxisToQuat(m_headingDegrees * DEG2RAD, localYAxis);
		// Multiply to obtain new orientation, with current orientation
        m_orientation = rotation * m_orientation;
    }
	// if turning in the x axis
	if (m_pitchDegrees != 0.0f)
    {
        rotation.ConvertAxisToQuat(m_pitchDegrees * DEG2RAD, localXAxis);
        m_orientation = rotation * m_orientation;
    }

    if (m_zDegrees != 0.0f)
    {
        rotation.ConvertAxisToQuat(m_zDegrees * DEG2RAD, localZAxis);
		// how this is multiplied counts so woch out
        m_orientation = rotation * m_orientation;
    }
}

/*
Update the view matrix

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].
*/
void Camera::UpdateViewMatrix(float dt)
{
	// obtain the orientation first, setting the first 3 columns in the matrix
	m_viewMatrix = m_orientation.SetMatrix();

	// Obtain the three different axes seperatly from the view matrix
    m_xAxis = Vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
    m_yAxis = Vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
    m_zAxis = Vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);

	 // Calculate the new camera position. The 'idealPosition' is where the
    // camera should be position. The camera should be positioned directly
    // behind the target at the required offset distance. Rather than have the camera 
	// immediately snap to the 'idealPosition'
    // it is slowly moveed towards the 'idealPosition' using a spring
    // system.
    //
    // References:
    //   Stone, Jonathan, "Third-Person Camera Navigation," Game Programming
    //     Gems 4, Andrew Kirmse, Editor, Charles River Media, Inc., 2004.

	// The ideal position is the position the camera "wants" to be at
	// This is worked out by adding the Current LookAt vector with the
	// distance to the object (where m_offsetDistance is the distance between
	// the camera and the object)
	Vec3 idealPosition = mLookAt + m_zAxis * m_offsetDistance;
	// Determine the difference between the current position and the ideal position position
    Vec3 displacement = mPosition - idealPosition;
	// Determine the acceleration to move the camera
    Vec3 springAcceleration = (-m_springConstant * displacement) - 
        (m_dampingConstant * mVelocity);
	// Obtain the Velocity and then move the camera by that velocity, to
	// allow the camera to slowly move towards the ideal position.
	mVelocity += springAcceleration * (dt*2);
	mPosition += mVelocity * (dt*2);

	 // The view matrix is always relative to the camera's current position
    // 'm_eye'. Since a spring system is being used here 'm_eye' will be
    // relative to 'idealPosition'. When the camera is no longer being
    // moved 'm_eye' will become the same as 'idealPosition'. The local
    // x, y, and z axes that were extracted from the camera's orientation
    // 'm_orienation' is correct for the 'idealPosition' only. We need
    // to recompute these axes so that they're relative to 'm_eye'. Once
    // that's done we can use those axes to reconstruct the view matrix.

	m_zAxis = mPosition - mLookAt;
	m_zAxis.Normalize();
	// View Direction is the inversed vector of local Z axis which is relative to the target poition
	m_viewDir = m_zAxis.Inverse();
	
	m_xAxis = m_viewDir.Cross(mUpVector);
    m_xAxis.Normalize();

	m_yAxis = m_xAxis.Cross(m_viewDir);
    m_yAxis.Normalize();
    m_xAxis.Normalize();

	/* Set the view matrix of the camera */ 
    m_viewMatrix[0][0] = m_xAxis.x;
    m_viewMatrix[1][0] = m_xAxis.y;
    m_viewMatrix[2][0] = m_xAxis.z;
	m_viewMatrix[3][0] = -m_xAxis.Dot(mPosition);

    m_viewMatrix[0][1] = m_yAxis.x;
    m_viewMatrix[1][1] = m_yAxis.y;
    m_viewMatrix[2][1] = m_yAxis.z;
	m_viewMatrix[3][1] = -m_yAxis.Dot(mPosition);

    m_viewMatrix[0][2] = m_zAxis.x;
    m_viewMatrix[1][2] = m_zAxis.y;
    m_viewMatrix[2][2] = m_zAxis.z;    
	m_viewMatrix[3][2] = -m_zAxis.Dot(mPosition);
}


