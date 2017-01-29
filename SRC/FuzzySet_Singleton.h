#ifndef __FUZZYSET_SINGLETON_H__
#define __FUZZYSET_SINGLETON_H__

/*
This defines a fuzzy set that is a singleton - a range over which the DOM is always 1.0

Author : Romesh Selvanathan

Referenced From : Programming Game AI by Example - Chapter 10
*/

#include "FuzzySet.h"

class FuzzySet_Singleton : public FuzzySet
{
public:
	FuzzySet_Singleton(float mid, float LeftOffset, float RighOffset);
	float CalculateDOM(float val) const;

private:
	/* Values that define the shap of this FLV */
	float m_fMidPoint;
	float m_fRightOffset;
	float m_fLeftOffset;
};

#endif