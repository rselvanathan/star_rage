/*
This class represent a matrix as a array m[4][4]. the first column in the array represent the
column in a matrix, the second the row.

It has been Directly referenced from the Demo below :
dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].

The modifications I have done are subtle and to do with using my own Vector class rather than
the one provided in the Demo

References : 

dhpoware. (2012). OpenGL Third Person Camera Demo. [online]. Available at: 
<http://www.dhpoware.com/demos/glThirdPersonCamera2.html.>[Accessed: 21 December 2011].

Fletcher, D and Parberry, I. (2002). 3D Math Primer for Graphics and Game Development. Texas: Wordware

*/
#ifndef __MATRIX4_H__
#define __MATRIX4_H__

class Vec3;
class Quaternion;

class Matrix4
{
public:
	Matrix4() {}
	Matrix4(float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44);
	~Matrix4() {}

	static const Matrix4 IDENTITY;

	void Identity();
	// Create a Matrix from a 3D vector
	void SetVector3(const Vec3 v);
	// create a rotation matrix with angle (rad) around axis of rotation vector
	void SetRotation(const Vec3 &axis, float degrees);
	// get euler values of rotation order head(y), pitch(x), roll(z) of matrix (Nasa standard)
	void GetEularAngles(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
	// set euler values of rotation order head(y), pitch(x), roll(z) of matrix (Nasa standard)
	void SetEularAngles(float headDegrees, float pitchDegrees, float rollDegrees);

	//////////////////////////////////Important Matrix functions/////////////////////////////////////

	// This will change a point in world space into local matrix space and return the vector
	Vec3 PointInLocalSpace(Vec3 vector);

	/////////////////////////////Operators/////////////////////////

	float *operator[](int row);
    const float *operator[](int row) const;

    bool operator==(const Matrix4 &rhs) const;
    bool operator!=(const Matrix4 &rhs) const;

    Matrix4 &operator+=(const Matrix4 &rhs);
    Matrix4 &operator-=(const Matrix4 &rhs);
    Matrix4 &operator*=(const Matrix4 &rhs);
    Matrix4 &operator*=(float scalar);
    Matrix4 &operator/=(float scalar);

    Matrix4 operator+(const Matrix4 &rhs) const;
    Matrix4 operator-(const Matrix4 &rhs) const;
    Matrix4 operator*(const Matrix4 &rhs) const;
    Matrix4 operator*(float scalar) const;
    Matrix4 operator/(float scalar) const;

	/////////////////////////////////////////////////////////////////

private:
	float mtx[4][4];
};

#endif 