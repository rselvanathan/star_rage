#ifndef __FUZZYSET_H__
#define __FUZZYSET_H__

/* 
A class to define an interface for a fuzzy set

Author : Romesh Selvanathan

Referenced From : Programming Game AI by Example - Chapter 10, pg 439
*/

#include "GameUtil.h"

class FuzzySet
{
public:
	FuzzySet(float RepVal) : m_fDOM(0.0f), m_fRepresentativeValue(RepVal){}

	/*
	Return the degree of membership in this set of given value.
	NOTE: This does not set the m_fDOM to the DOm value passed as parameter. This is
	becuase the centroid defuzzification method also uses this method to detrmine the DOMS of the value
	it uses as its sample points
	*/
	virtual float CalculateDOM(float val) const = 0;

	/*
	If this fuzzy set is part of a consequent FLV and is fired by a rule = DOM is set (in this context
	DOM = confidence level) to the max of the paramter value or the set's existing m_fDOM value
	*/
	void  ORwithDOM(float val) {if (val> m_fDOM) m_fDOM = val;}

	/* Accessor methods */
	float GetRepresentativeVal() const {return m_fRepresentativeValue;}
	float GetDOM() const {return m_fDOM;}

	/* Mutator methods */
	void ClearDOM() {m_fDOM = 0.0f;}
	void SetDOM(float f){
		assert((f <= 1) && (f >= 0) && "<FuzzySet::SetDOM>: invalid value");
		m_fDOM = f;
	}

protected:
	/* this is the degree of membership of a given value in this set */
	float m_fDOM;

	/*
	This is the maximum of the set's membership function. For ex, if the set is triangular
	then this will be the peak point of the triangular. IF the set has a plateau then thi will be the mid-points of the plateau. 
	*/
	float m_fRepresentativeValue;
};

#endif