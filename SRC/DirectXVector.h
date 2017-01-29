/*
A Vector Class that uses the DirectX SDK to simply the implementation, by encapsulating and wrapping
the DirectX Vector classes

Author : Romesh Selvanathan
Data : 03/05/12

References :

McShaffry, M. (2009). Game Coding Complete. 3rd ed. Boston: Cengage Learning.
*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "GameUtil.h"

// A 2D Vector
class Vec2 : public D3DXVECTOR2
{
public:
	// Contructor
	Vec2() : D3DXVECTOR2() {}
	// Constructor with another DirectX Vector
	Vec2(D3DXVECTOR2 &v2){x = v2.x; y = v2.y;}
	// Specify the components
	Vec2(const float _x, const float _y){x =_x; y = _y;}
	//inline Vec2(const class Vec3 &v3) {x = v3.x; y = v3.y;}
	//inline Vec2(const class Vec4 &v4) {x = v4.x; y = v4.y;}

	// Obtain the length of the vector
	inline float Length(){return D3DXVec2Length(this);}
	// Normalize the vector
	inline Vec2* Normalize(){return static_cast<Vec2*>(D3DXVec2Normalize(this, this));}
	// Obtain the dot product with another 2D vector
	inline float Dot(const Vec2 &b){return D3DXVec2Dot(this, &b);}
};

// A 3D vector 
class Vec3 : public D3DXVECTOR3
{
public:
	//Constructor
	Vec3() : D3DXVECTOR3() {}
	// Constructor by providing another vector
	Vec3(D3DXVECTOR3 &v3){x = v3.x; y = v3.y; z = v3.z;}
	// Constructor by providing the components
	Vec3(const float _x, const float _y, const float _z){x =_x; y = _y; z = _z;}
	// Constrctor with a 2D vector
	inline Vec3(const class Vec2 &v2) {x = v2.x; y = v2.y; z = 1.0f;}
	//inline Vec3(const class Vec4 &v4) {x = v4.x; y = v4.y; z = v4.z;}
	// Inverse the 3D vector
	inline Vec3 Inverse() { return Vec3(-x, -y, -z);}
	// Length of the vector
	inline float Length(){return D3DXVec3Length(this);}
	// the Normalize the vector
	inline Vec3* Normalize(){return static_cast<Vec3*>(D3DXVec3Normalize(this, this));}
	// THe Dot product of the vector with another
	inline float Dot(const Vec3 &b){return D3DXVec3Dot(this, &b);}
	// the cross product of this vector and another
	inline Vec3 Cross(const Vec3 &b) const
	{
		Vec3 out;
		D3DXVec3Cross(&out, this, &b);
		return out;
	}
};

// A 4D vector
class Vec4 : D3DXVECTOR4
{
public:
	//Constructor
	Vec4() : D3DXVECTOR4() {}
	// Constructor by providing another vector
	Vec4(D3DXVECTOR4 &v4){x = v4.x; y = v4.y; z = v4.z; w = v4.w;}
	// Constructor by providing the components
	Vec4(const float _x, const float _y, const float _z, const float _w)
	{x =_x; y = _y; z = _z; w = _w;}
	// Constrctor with a 2D vector
	inline Vec4(const class Vec2 &v2) {x = v2.x; y = v2.y; z = 1.0f; w = 1.0f;}
	// Constrctor with a 3D vector
	inline Vec4(const class Vec3 &v3) {x = v3.x; y = v3.y; z = v3.z; w = 1.0f;}
	// Length of the vector
	inline float Length(){return D3DXVec4Length(this);}
	// the Normalize the vector
	inline Vec4* Normalize(){return static_cast<Vec4*>(D3DXVec4Normalize(this, this));}
	// THe Dot product of the vector with another
	inline float Dot(const Vec4 &b){return D3DXVec4Dot(this, &b);}
};

#endif