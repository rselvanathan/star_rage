#ifndef __FUZZYOPERATORS_H__
#define __FUZZYOPERATORS_H__

/*
Classes to provide fuzzy AND and OR operators to be used in creation of a fuzzy rule base

Author : Mat Buckland (www.ai-junkie.com)

Referenced From : Programming Game AI by Example - Chapter 10
*/

#include "GameUtil.h"
#include "FuzzyTerm.h"

//--------------------------------------AND OPERATOR----------------------------------------

class FzAND : public FuzzyTerm
{
public:
	~FzAND();

	/*copy ctor*/
	FzAND(const FzAND& fa);
   
	/*ctors accepting fuzzy terms.*/
	FzAND(FuzzyTerm& op1, FuzzyTerm& op2);
	FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3);
	FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4);

	/*virtual ctor*/
	FuzzyTerm* Clone()const{return new FzAND(*this);}
  
	float GetDOM()const;
	void  ClearDOM();
	void  ORwithDOM(float val);
private:
  /*an instance of this class may AND together up to 4 terms */
  vector<FuzzyTerm*> m_Terms;

  /* disallow assignment */
  FzAND& operator=(const FzAND&);
};

//---------------------------------------OR operator---------------------------------
class FzOR : public FuzzyTerm
{
public:

  ~FzOR();

  /*copy ctor*/
  FzOR(const FzOR& fa);
   
  //ctors accepting fuzzy terms.
  FzOR(FuzzyTerm& op1, FuzzyTerm& op2);
  FzOR(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3);
  FzOR(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4);

  //virtual ctor
  FuzzyTerm* Clone()const{return new FzOR(*this);}
  
  float GetDOM()const;

  //unused
  void ClearDOM(){assert(0 && "<FzOR::ClearDOM>: invalid context");}
  void ORwithDOM(float val){assert(0 && "<FzOR::ORwithDOM>: invalid context");}

private:
  //an instance of this class may AND together up to 4 terms
  vector<FuzzyTerm*> m_Terms;

  //no assignment op necessary
  FzOR& operator=(const FzOR&);

};


#endif