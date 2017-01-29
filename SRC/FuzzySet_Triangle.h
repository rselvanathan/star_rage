#ifndef __FUZZYSET_TRIANGLE_H__
#define __FUZZYSET_TRIANGLE_H__

/*
Simple class to define fuzzy sets that have a triangular shape and can be defined
by a midpoint , a left displacement and a right displacement.

Author : Romesh Selvanathan

Referenced From : Programming Game AI by Example - Chapter 10
*/

#include "FuzzySet.h"

class FuzzySet_Triangle : public FuzzySet
{
public:
	FuzzySet_Triangle(float mid, float LeftOffset, float RighOffset);
	float CalculateDOM(float val) const;

private:
	/* Values that define the shap of this FLV */
	float m_fPeakPoint;
	float m_fRightOffset;
	float m_fLeftOffset;
};

#endif