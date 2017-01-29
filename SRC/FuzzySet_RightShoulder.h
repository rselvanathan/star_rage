#ifndef __FUZZYSET_RIGHTSHOULDER_H__
#define __FUZZYSET_RIGHTSHOULDER_H__

/*
Definition of a fuzzy set that has a right shoulder shape. - The maximum value this variable can 
accept is "any" value less thanthe midpoint

Author : Romesh Selvanathan

Referenced From : Programming Game AI by Example - Chapter 10
*/

#include "FuzzySet.h"

class FuzzySet_RightShoulder : public FuzzySet
{
public:
	FuzzySet_RightShoulder(float peak, float LeftOffset, float RighOffset);
	float CalculateDOM(float val) const;

private:
	/* Values that define the shap of this FLV */
	float m_fPeakPoint;
	float m_fRightOffset;
	float m_fLeftOffset;
};

#endif