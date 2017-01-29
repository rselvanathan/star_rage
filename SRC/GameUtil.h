#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glext.h>
#include <d3dx9.h>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <assert.h>
#include <math.h>
#include <cmath>
#include <algorithm>
#include "GLVector.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "DirectXVector.h"
#include "SmartPtr.h"
#include "MyException.h"
#include "Graphics.h"

typedef unsigned int uint;
typedef unsigned char uchar;
typedef _LARGE_INTEGER largeInt;

// Linking in the DirectX library required to compile
#pragma comment(lib,"d3dx9.lib")

using namespace std;
using namespace idllib;

// Few useful constants
const float  MaxFloat = (std::numeric_limits<float>::max)();
const float  MinFloat = (std::numeric_limits<float>::min)();
static const float Epsilon = 1e-6f;

 inline bool closeEnough(float f1, float f2)
 {
        // Determines whether the two floating-point values f1 and f2 are
        // close enough together that they can be considered equal.

       return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < Epsilon;
 }

 //returns the minimum of two values
template <class T>
inline T MinOf(const T& a, const T& b)
{
  if (a<b) return a; return b;
}

// Clamps the First argument with the second two
// Referenced From Programming Game AI by Example - Raven Source Code
template <class T, class U, class V>
inline void Clamp(T& arg, const U& minVal, const V& maxVal)
{
	assert( ((double)minVal < (double)maxVal) && "<Clamp>MaxVal <MinVal!");

	if (arg < (T) minVal)
		arg = (T)minVal;
	if (arg > (T) maxVal)
		arg = (T)maxVal;
} 

// Used for the sky box rendering
#define GL_CLAMP_TO_EDGE	0x812F

// If M_PI has not been defined in math.h add it now
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define value to convert angles in degrees to radians and vice versa
#define DEG2RAD (M_PI/180)
#define RAD2DEG (180/M_PI)

// A helper function to delete a pointer safely
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

// Two Helper macros that set Blending and Texture 2D on and off (Repeatedly used)
#define BLEND_TEX_ON	glEnable(GL_TEXTURE_2D);glEnable(GL_BLEND);glDisable(GL_DEPTH_TEST);glBlendFunc(GL_SRC_ALPHA, GL_ONE);  
#define BLEND_TEX_OFF   glBlendFunc(GL_ONE, GL_ONE);glEnable(GL_DEPTH_TEST);glDisable(GL_BLEND);glDisable(GL_TEXTURE_2D);

#define WINDOW_WIDTH 1280 
#define WINDOW_HEIGHT 1024

#endif