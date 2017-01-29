#ifndef __PROXY_FUZZY_SET_H__
#define __PROXY_FUZZY_SET_H__

/*
class to provide a proxy for a fuzzy set. The proxy inherits from 
FuzzyTerm and therefore can be used to create fuzzy rules

A Proxy class is used to hide the real class from the client. It acts as a surrogate to the
real class in order to control access to it. Proxy classes maintain a reference to the class they are a
surrogate for and when a client class a method of the proxy, it forwards the calss to the equivalent
method of the reference.

Whenever FuzzySet is added to a fuzzy variable the client is handed a proxy to it in the form of FzSet.
This proxy can be copied and used multiple times in the creation of the rule base. No moatter how many times
it is copied it will always surrogate for the same object, which tidies up the design considerably since we 
don't have to worry about keeping track of copies of FuzzySets as rules are created.

Author: Mat Buckland (www.ai-junkie.com)

Referenced from : Programming Game AI by Example - Chapter 10

*/

#include "FuzzyTerm.h"
#include "FuzzySet.h"

class FzAND;

class FzSet : public FuzzyTerm
{
private:
  
  //let the hedge classes be friends 
  friend class FzVery;
  friend class FzFairly;

private:

  //a reference to the fuzzy set this proxy represents
  FuzzySet& m_Set;

public:

  FzSet(FuzzySet& fs):m_Set(fs){}

  FuzzyTerm* Clone()const{return new FzSet(*this);}
  float     GetDOM()const {return m_Set.GetDOM();}
  void       ClearDOM(){m_Set.ClearDOM();}
  void       ORwithDOM(float val){m_Set.ORwithDOM(val);}
};


#endif
