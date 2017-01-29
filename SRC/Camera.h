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

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "GameUtil.h"
#include "Quaternion.h"

class Camera
{
public:
	// Constructor
	Camera();
	// Destructor
	~Camera();
	// Reset the camera 
	void Reset();
	// Update the camera at every frame update
	void Update(float dt);
	// Update the Orientation of the Camera
	void UpdateOrientation(float dt);
	// Update the view matrix of the camera
	void UpdateViewMatrix(float dt);
	
	// Accessor Methods
	const float GetRotX() const{return m_pitchDegrees;}
	const float GetRotY() const{return m_headingDegrees;}
	//const float GetRotZ() const{return mZRot;}
	const Vec3 GetPosition() const{return mPosition;}
	const Vec3 GetLookAt() const{return mLookAt;}
	const Vec3 GetViewDirection() const {return m_viewDir;}
	const Vec3 GetUpVec() const{return mUpVector;}
	const Matrix4 GetViewMatrix()const{return m_viewMatrix;}
	const float GetDampingConst() const {m_dampingConstant;}
	float GetFov() {return fov;}

	// Mutator methods
	void SetSpringConstant(float constant);
	void SetDampingConstant(float damp) {m_dampingConstant = damp;}
	void SetLookAt(const Vec3 &target);
	void SetLookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up); 
	void SetRotate(float longitudeDegrees, float latitudeDegrees, float z);
	void SetRotateX(float pitch) { m_pitchDegrees = -pitch;}
	void SetRotateY(float yaw) { m_headingDegrees = -yaw;}
	void SetRotateZ(float roll) {m_zDegrees = -roll;}
	void SetTargetMat(Matrix4 mat){m_targetMatrix = mat;}
	void SetSprint(bool s) {m_sprint = s;}

	void SetPosition(const Vec3 &vec){mPosition = vec;}
	void SetUpVector(const Vec3 &vec){mUpVector = vec;}

private:
	Vec3 mPosition;				// Position of Camera in the world
	Vec3 mVelocity;				// Velocity of camera to turn towards the target slowly (allows smooth turning)

	Vec3 mLookAt;				// The look at vector(Z)
	Vec3 mUpVector;				// The up Vector for the camera(Y)

	float m_offsetDistance;						// The offset distance, which is the distance between the target and the camera
	float m_springConstant;						// Spring value to smooth out camera movement
	float m_dampingConstant;					// Damp value to smooth out camera movement
	float m_headingDegrees;						// Rotation in Y-axis
    float m_pitchDegrees;						// Rotation in X-Axis
	float m_zDegrees;							// Rotation in Z_Axis
    Vec3 m_offset;								// Offset vector (VERY IMP)
    Vec3 m_xAxis;								// local camera x-axis
    Vec3 m_yAxis;								// local camera y-axis
    Vec3 m_zAxis;								// local camera z-axis
    Vec3 m_viewDir;								// View Direction of camera
    Matrix4 m_viewMatrix;						// The view matrix which produced for viewing the world
	Matrix4 m_targetMatrix;						// The matrix of the target
    Quaternion m_orientation;					// The orientation for the camera, avoid gimbal lock

	bool m_sprint;								// the sprint flag
	float fov;									// the Field of view expansion
	float shake;								// This value allows camera to shake when sprinting
	float gradualShake;							// the gradual shake value that slowly increments the value
};

#endif