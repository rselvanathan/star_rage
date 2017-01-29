/*
This class represents a Quaternion which can be used for rotations.

Author : Romesh Selvanathan
Date : 03/05/12

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].

Fletcher, D and Parberry, I. (2002). 3D Math Primer for Graphics and Game Development. Texas: Wordware

*/

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "GameUtil.h"
#include <cmath>

#define TOLERANCE 0.00001f

class Matrix4;
class Vec3;

class Quaternion
{
public:
	// Default Quaternion
	Quaternion();
	// Quartenion with sepcified values(w in radians, and this will be normalized)
	Quaternion(float _w,float _x, float _y, float _z);
	// Quaternion with axis-angle representation (w in radians)
	Quaternion(const float angle, const Vec3& v);
	// Quaternion with Eular Angles representation (in degrees)
	Quaternion(const float headDegrees, const float pitchDegrees, const float rollDegrees);
	// Quaternion with Matrix
	Quaternion(const Matrix4 &m);

	static const Quaternion IDENTITY;

	void Identity();
	void Normalize();
	Quaternion Conjugate() const;
	Quaternion Inverse() const;

	void ConvertAxisToQuat(const float angle, const Vec3& v);
	void ConvertEularToQuat(const float x, const float y, const float z);
	void ConvertEularToQuat(const Matrix4& m, const float pitchDegrees, const float headDegrees, const float rollDegrees);
	void ExtractOrientationAxis(Vec3& right, Vec3& up, Vec3& forward); 

	void GetValues(float &w, float &x, float &y, float &z) const; 
	//void GetEularAngles(float &x, float &y, float &z) const;		Uses Matrix so left it for now
	void GetAxisAngle(float &angle, Vec3& v) const;
	void GetSlerpQuat(Quaternion &result, Quaternion q2, float t);

	//Matrix
	void SetHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
	void GetHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
	Matrix4 SetMatrix() const;
	void GetMatrix(const Matrix4 &m);

	/*operators*/
	bool operator==(const Quaternion &rhs) const;
    bool operator!=(const Quaternion &rhs) const;

    Quaternion &operator+=(const Quaternion &rhs);
    Quaternion &operator-=(const Quaternion &rhs);
    Quaternion &operator*=(const Quaternion &rhs);
    Quaternion &operator*=(float scalar);
    Quaternion &operator/=(float scalar);

    Quaternion operator+(const Quaternion &rhs) const;
    Quaternion operator-(const Quaternion &rhs) const;
    Quaternion operator*(const Quaternion &rhs) const;
    Quaternion operator*(float scalar) const;
    Quaternion operator/(float scalar) const;

	Vec3 operator*(const Vec3 &vec) const;

	float	w, x, y, z;
};

#endif