/*
This class represents a Quaternion which can be used for rotations.

Author : Romesh Selvanathan
Date : 03/05/12

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].

Fletcher, D and Parberry, I. (2002). 3D Math Primer for Graphics and Game Development. Texas: Wordware

*/ 
#include "Quaternion.h"
#include "Matrix4.h"

const Quaternion Quaternion::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);

/* Default Constructor */ 
Quaternion::Quaternion()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

/* Quartenion with sepcified values(w in radians, and this will be normalized) */ 
Quaternion::Quaternion(float _w,float _x, float _y, float _z) 
	: w(_w), x(_x), y(_y), z(_z)
{
	//this->x = x; this->y = y; this->z = z; this->w = w;

	//Normalize();
}

/* Quaternion with Eular Angles representation (in degrees) */ 
//Quaternion::Quaternion(const float x, const float y, const float z)
//{
//	ConvertEularToQuat(x, y, z);
//}

/* Quaternion with axis-angle representation (w in radians) */ 
Quaternion::Quaternion(const float angle, const Vec3& v)
{
	ConvertAxisToQuat(angle, v);
}

 Quaternion::Quaternion(const float headDegrees, const float pitchDegrees, const float rollDegrees)
{
    SetHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
}

 Quaternion::Quaternion(const Matrix4 &m)
{
    GetMatrix(m);
}

/* The Identity values of the Quaternion */ 
void Quaternion::Identity()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

/* Normalizing the Quaternion works very similarly to a Vector. It the Quaternion
   is close to the unit - length then this method will not do anything. TOLERANCE 
   is the check to see if it is close enough to the unit - length*/ 
void Quaternion::Normalize()
{
	// Magnitude of the Quaternion (size)
	float mag = (w*w) + (x*x) + (y*y) + (z*z);
	//Check if the value is close enough to the unit- length
	if(fabs(mag) > TOLERANCE && fabs(mag -1.f) > TOLERANCE)
	{
		//if not
		// square root the magnitude to obtain length(just like the vector method of normalizing)
		float length = sqrt(mag);
		// divide each component by the length
		w /= length;
		x /= length;
		y /= length;
		z /= length;
	}
}

// Returns the Quatornian with negative values.
Quaternion Quaternion::Conjugate() const
{
    Quaternion tmp(w, -x, -y, -z);
    return tmp;
}

Quaternion Quaternion::Inverse() const
{
	float mag = sqrtf(w * w + x * x + y * y + z * z);
    float invMag = 1.0f / mag;
    return Conjugate() * invMag;
}

/* Converting a axis-angle representation to a quaternion.
   The formula for converting to quaternion from an rotation about
   and angle 'A' about axis(unit vector) V is
   q = cos(A/2) + V*sin(A/2) */ 
void Quaternion::ConvertAxisToQuat(const float angle, const Vec3& v)
{
	// temp angle A / 2
	float tempAngle = angle/2;
	// temp Vector
	Vec3 tempVec(v);
	// Unit-Vector if temp
	tempVec.Normalize();
	// the sin(A/2) value
	float sinAngle = sin(tempAngle);

	// the V*sin(A/2) values for each component in a vector
	x = (tempVec.x * sinAngle); 
	y = (tempVec.y * sinAngle); 
	z = (tempVec.z * sinAngle);
	// the cos(A/2) value
	w = cos(tempAngle);

	//Normalize to make sure unit-quat is returned
	//Normalize();
}

/* Convert from Eular angle (degrees) representation to Quaternions
   Basically we create 3 Quartenions, one for x(pitch), one for yaw(y)
   and one for z(roll) and multiply them together as (Q1 * Q2 * Q3) to 
   obtain a single quaternion with the combined rotations.
   The calculation below does the same however it is shorter.*/ 
void Quaternion::ConvertEularToQuat(const float x, const float y, const float z)
{
	// convert angles to radians then half them
	float halfx = (x*DEG2RAD) / 2.f;
	float halfy = (y*DEG2RAD) / 2.f;
	float halfz = (z*DEG2RAD) / 2.f;

	// obtain the cos values for each halfed value
	float cosX = cos(halfx);
	float cosY = cos(halfy);
	float cosZ = cos(halfz);

	// obtain the sin values for each halfed value
	float sinX = sin(halfx);
	float sinY = sin(halfy);
	float sinZ = sin(halfz);

	// do the shorter version of multiplying and obtaining a single quaternion
	// out of the 3 rotations.
	this->x = (sinZ * cosX * cosY) - (cosZ * sinX * sinY);
	this->y = (cosZ * sinX * cosY) + (sinZ * cosX * sinY);
	this->z = (cosZ * cosX * sinY) - (sinZ * sinX * cosY);
	this->w = (cosZ * cosX * cosY) + (sinZ * sinX * sinY);

	// Normalize the values
	Normalize();
}

// Converting Euler Angles into Quaternion - Basically a Quaternion into a matrix or use a rotation matrix
// from which extracting all the 3 axes you can obtain the new Quaternion
void Quaternion::ConvertEularToQuat(const Matrix4& m, const float pitchDegrees, const float headingDegrees, const float rollDegrees)
{
	// Construct a quaternion from an euler transformation. We do this rather
    // than use Quaternion::fromHeadPitchRoll() to support constraining heading
    // changes to the world Y axis.

	this->Identity();
    Quaternion rotation = Quaternion::IDENTITY;
    Vec3 localXAxis(m[0][0], m[0][1], m[0][2]);
    Vec3 localYAxis(m[1][0], m[1][1], m[1][2]);
    Vec3 localZAxis(m[2][0], m[2][1], m[2][2]);   

    if (headingDegrees != 0.0f)
    {
		// In order to allow free rotation in any direction have to use the local Y axis rather than a global one, where only rotations in one direction is allowed
		// so for ex if I roll 90 degrees and then want to rotate I am stil using the world Y axis rather than local, so it would seem tham I am looking up and down rather than
		// sideways. Se this below is the wrong one to use 
		//rotation.ConvertAxisToQuat(headingDegrees * DEG2RAD,Vec3(0.0f, 1.0f, 0.0f));

		rotation.ConvertAxisToQuat(headingDegrees * DEG2RAD, localYAxis);
        (*this) *= rotation;
    }

    if (pitchDegrees != 0.0f)
    {
        rotation.ConvertAxisToQuat(pitchDegrees * DEG2RAD, localXAxis);
        (*this) *= rotation;
    }

    if (rollDegrees != 0.0f)
    {
        rotation.ConvertAxisToQuat(rollDegrees * DEG2RAD, localZAxis);
        (*this) *= rotation;
    }
}

// Extract and obtain the three orientation axis by turning this quaternion into a matrix first
void Quaternion::ExtractOrientationAxis(Vec3& right, Vec3& up, Vec3& forward)
{
	Matrix4 m = this->SetMatrix();

    right = Vec3(m[0][0], m[0][1], m[0][2]);
    right.Normalize();
    
    up = Vec3(m[1][0], m[1][1], m[1][2]);
    up.Normalize();
    
    forward = Vec3(-m[2][0], -m[2][1], -m[2][2]);
    forward.Normalize();
}

/* Return the current values of the quaternion */
void Quaternion::GetValues(float &w, float &x, float &y, float &z) const
{
	w = this->w;
	x = this->x;
	y = this->y;
	z = this->z;
}

/* Convert from quaternion to axis/angle representation 
   To find angle of rotation its : w = cos(A/2)
   and for non-normalized rotation axis the vector is simply
   Qv from quaternion where q = (w, Qv) ~ Qv = (Qx, Qy, Qz)*/ 
void Quaternion::GetAxisAngle(float &angle, Vec3& v) const
{
	// length of x y z of quaternion
	//float length = (float)sqrt(x*x + y*y + z*z);
	float sinHalfThetaSq = 1.f - (w* w);
	/* if scale is 0 then set angle to 0 and return vector(0, 0, 1)
	   this is because is length is - that measn there is no rotation
	   which will cause the ais to be infinite(BAD!) */ 
	//if(length == 0.f) {
	if(sinHalfThetaSq <= 0.f) {
		angle = 0.0f;

		v.x = 1.0f;
		v.y = 0.0f;
		v.z = 0.0f;
	/* else angle = acos(w) * 2.f (opposite of w = cos(A/2)) and
	   vector is each component of Qv / length to = each component of vector
	   and at last normalize the vector. */ 
	} else {
		// angle in radians
		angle = (float)acos(w) * 2.f;
		float length = 1.f/sqrt(sinHalfThetaSq);

		v.x = x * length;///length;
		v.y = y * length;///length;
		v.z = z * length;///length;
		//v.Normalize();
	}
}

// Reference from Chapter 10 " Orientation and Angular Displacement in 3D" pg 176,
// Book "3D Math Primer for Graphics and Game Development " , Fletcher Dunn and Ian Parberry
void Quaternion::GetSlerpQuat(Quaternion &result, Quaternion q2, float t)
{
	float cosOmega = this->w * q2.w + this->x * q2.x * this->y * q2.y + this->z * q2.z;

	if(cosOmega < 0.0f) {
		q2.w = -q2.w;
		q2.x = -q2.x;
		q2.y = -q2.y;
		q2.z = -q2.z;
		cosOmega  = -cosOmega;
	}

	float k0, k1;
	if( cosOmega > 0.9999f){
		k0 = 1.0f - t;
		k1 = t;
	} else {
		float sinOmega = sqrt(1.0f - cosOmega* cosOmega);
		float omega = atan2(sinOmega, cosOmega);
		float oneOverSingOmega = 1.0f / sinOmega;
		k0 = sin((1.0f - t)*omega)*oneOverSingOmega;
		k1 = sin(t * omega) * oneOverSingOmega;
	}

	result.w = this->w * k0 + q2.w * k1;
	result.x = this->x * k0 + q2.x * k1;
	result.y = this->y * k0 + q2.y * k1;
	result.z = this->z * k0 + q2.z * k1;
}

//////////////////////////////////////////////////////Matrix/////////////////////////////////////////

void Quaternion::SetHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
{
    Matrix4 m;
    m.SetEularAngles(headDegrees, pitchDegrees, rollDegrees);
    GetMatrix(m);
}

void Quaternion::GetHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const
{
    Matrix4 m = SetMatrix();
    m.GetEularAngles(headDegrees, pitchDegrees, rollDegrees);
}

  /*Creates a quaternion from a rotation matrix. 
  The algorithm used is from Allan and Mark Watt's "Advanced 
   Animation and Rendering Techniques" (ACM Press 1992). */ 
void Quaternion::GetMatrix(const Matrix4 &m)
{
    float s = 0.0f;
    float q[4] = {0.0f};
    float trace = m[0][0] + m[1][1] + m[2][2];

    if (trace > 0.0f)
    {
        s = sqrtf(trace + 1.0f);
        q[3] = s * 0.5f;
        s = 0.5f / s;
        q[0] = (m[1][2] - m[2][1]) * s;
        q[1] = (m[2][0] - m[0][2]) * s;
        q[2] = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        int nxt[3] = {1, 2, 0};
        int i = 0, j = 0, k = 0;

        if (m[1][1] > m[0][0])
            i = 1;

        if (m[2][2] > m[i][i])
            i = 2;

        j = nxt[i];
        k = nxt[j];
        s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);

        q[i] = s * 0.5f;
        s = 0.5f / s;
        q[3] = (m[j][k] - m[k][j]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;
    }

    x = q[0], y = q[1], z = q[2], w = q[3];
}

/* Converts this quaternion to a rotation matrix.
    
      | 1 - 2(y^2 + z^2)	2(xy + wz)			2(xz - wy)			0  |
      | 2(xy - wz)		1 - 2(x^2 + z^2)	2(yz + wx)			0  |
      | 2(xz + wy)		2(yz - wx)			1 - 2(x^2 + y^2)	0  |
	  | 0					0					0					1  |
	  */ 
Matrix4 Quaternion::SetMatrix() const
{
    float x2 = x + x; 
    float y2 = y + y; 
    float z2 = z + z;
    float xx = x * x2;
    float xy = x * y2;
    float xz = x * z2;
    float yy = y * y2;
    float yz = y * z2;
    float zz = z * z2;
    float wx = w * x2;
    float wy = w * y2;
    float wz = w * z2;

    Matrix4 m;

    m[0][0] = 1.0f - (yy + zz);
    m[0][1] = xy + wz;
    m[0][2] = xz - wy;
    m[0][3] = 0.0f;

    m[1][0] = xy - wz;
    m[1][1] = 1.0f - (xx + zz);
    m[1][2] = yz + wx;
    m[1][3] = 0.0f;

    m[2][0] = xz + wy;
    m[2][1] = yz - wx;
    m[2][2] = 1.0f - (xx + yy);
    m[2][3] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;

    return m;
}

//////////////////////////////////////////////////////operators//////////////////////////////////////

Quaternion operator*(float lhs, const Quaternion &rhs)
{
    return rhs * lhs;
}

bool Quaternion::operator==(const Quaternion &rhs) const
{
	return closeEnough(w, rhs.w) && closeEnough(x, rhs.x)
        && closeEnough(y, rhs.y) && closeEnough(z, rhs.z);
} 

bool Quaternion::operator!=(const Quaternion &rhs) const
{
    return !(*this == rhs);
}

Quaternion Quaternion::operator*(const Quaternion &rhs) const
{
    Quaternion tmp(*this);
    tmp *= rhs;
    return tmp;
}

Quaternion &Quaternion::operator+=(const Quaternion &rhs)
{
    w += rhs.w, x += rhs.x, y += rhs.y, z += rhs.z;
    return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &rhs)
{
    w -= rhs.w, x -= rhs.x, y -= rhs.y, z -= rhs.z;
    return *this;
}

Quaternion &Quaternion::operator*=(const Quaternion &rhs)
{
    // Multiply so that rotations are applied in a left to right order.
    Quaternion tmp(
        (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
        (w * rhs.x) + (x * rhs.w) - (y * rhs.z) + (z * rhs.y),
        (w * rhs.y) + (x * rhs.z) + (y * rhs.w) - (z * rhs.x),
        (w * rhs.z) - (x * rhs.y) + (y * rhs.x) + (z * rhs.w));
    
    /*// Multiply so that rotations are applied in a right to left order.
    Quaternion tmp(
    (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
    (w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y),
    (w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x),
    (w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w));
    */ 

    *this = tmp;
    return *this;
}

Quaternion &Quaternion::operator*=(float scalar)
{
    w *= scalar, x *= scalar, y *= scalar, z *= scalar;
    return *this;
}

Quaternion &Quaternion::operator/=(float scalar)
{
    w /= scalar, x /= scalar, y /= scalar, z /= scalar;
    return *this;
}

Quaternion Quaternion::operator+(const Quaternion &rhs) const
{
    Quaternion tmp(*this);
    tmp += rhs;
    return tmp;
}

Quaternion Quaternion::operator-(const Quaternion &rhs) const
{
    Quaternion tmp(*this);
    tmp -= rhs;
    return tmp;
}

Quaternion Quaternion::operator*(float scalar) const
{
    Quaternion tmp(*this);
    tmp *= scalar;
    return tmp;
}

Quaternion Quaternion::operator/(float scalar) const
{
    Quaternion tmp(*this);
    tmp /= scalar;
    return tmp;
}


Vec3 Quaternion::operator* (const Vec3 &vec) const
{
	Vec3 vn(vec);
	vn.Normalize();
 
	Quaternion vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;
 
	resQuat = vecQuat * Conjugate();
	resQuat = *this * resQuat;
 
	return (Vec3(resQuat.x, resQuat.y, resQuat.z));
}

