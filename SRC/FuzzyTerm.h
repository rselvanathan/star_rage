#ifndef __FUZZYTERM_H__
#define __FUZZYTERM_H__

/*
Abstract class to provide interface for classes able to be used as terms in a fuzzy 
if - then rule base

Author: Mat Buckland (www.ai-junkie.com)

Referenced from : Programming Game AI by Example - Chapter 10
*/

class FuzzyTerm
{
public:
	virtual ~FuzzyTerm() {}
	/* Constructor */
	virtual FuzzyTerm* Clone() const = 0;
	/* Ontains degree of membership of the term */
	virtual float GetDOM() const = 0;
	/* Clears the degree of Membership of the term */
	virtual void ClearDOM() = 0;
	/* Method fir updating the DOM if a consequent when a fule fires */
	virtual void ORwithDOM(float val) = 0;
};

#endif