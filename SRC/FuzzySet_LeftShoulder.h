#ifndef __FUZZYSET_LEFTSHOULDER_H__
#define __FUZZYSET_LEFTSHOULDER_H__

/*
Definition of a fuzzy set that has a left shoulder shape. - The minimum value this variable can accept is "any" value less than
the midpoint

Author : Romesh Selvanathan

Referenced From : Programming Game AI by Example - Chapter 10
*/

#include "FuzzySet.h"

class FuzzySet_LeftShoulder : public FuzzySet
{
public:
	FuzzySet_LeftShoulder(float peak, float LeftOffset, float RighOffset);
	float CalculateDOM(float val) const;

private:
	/* Values that define the shap of this FLV */
	float m_fPeakPoint;
	float m_fRightOffset;
	float m_fLeftOffset;
};

#endif