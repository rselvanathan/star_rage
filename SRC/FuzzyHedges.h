#ifndef __FUZZY_HEDGES_H__
#define __FUZZY_HEDGES_H__

/*
Classes to implement fuzzy Hedges - These are unary operators that can be employed
to modify the meaning of a fuzzy Set - For example VERY or Fairly :
VERY modifies it like so : VERY(F_a) = (F_a(x)) * (F_a(x))
Fairly modifies it like so : FAIRLY(F_a) = sqrt(F_a(x))

Author: Mat Buckland (www.ai-junkie.com)

Referenced From : Programming Game AI by Example - Chapter 10
*/

#include "FuzzySet.h"
#include "FuzzyTerm.h"
#include "FzSet.h"

class FzVery : public FuzzyTerm
{
public:
	FzVery(FzSet& ft) : m_Set(ft.m_Set){}
	float GetDOM() const {return m_Set.GetDOM() * m_Set.GetDOM();} 
	FuzzyTerm* Clone() const {return new FzVery(*this);}

	void ClearDOM() {m_Set.ClearDOM();}
	void ORwithDOM(float val) {m_Set.ORwithDOM(val * val);}

private:
	FuzzySet& m_Set;
	// Stops copying and assignment by clients
	FzVery(const FzVery& inst) : m_Set(inst.m_Set){}
	FzVery& operator=(const FzVery&);
};

//------------------------------FzFairly----------------------------------

class FzFairly : public FuzzyTerm
{
public:
	FzFairly(FzSet& ft) : m_Set(ft.m_Set){}
	float GetDOM() const {return sqrt(m_Set.GetDOM());}
	FuzzyTerm* Clone() const {return new FzFairly(*this);}

	void ClearDOM() {m_Set.ClearDOM();}
	void ORwithDOM(float val) {m_Set.ORwithDOM(sqrt(val));}
private:
	FuzzySet& m_Set;
	// Stops copying and assignment by clients
	FzFairly(const FzFairly& inst) : m_Set(inst.m_Set){}
	FzFairly& operator=(const FzVery&);
};

#endif