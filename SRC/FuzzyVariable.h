#ifndef __FUZZYVARIABLE_H__
#define __FUZZYVARIABLE_H__

/*
Class to define a Fuzzy linguistic variable(FLV) . An FLV comprises of a number of fuzzy sets

Author : Mat Buckland (www.ai-junkie.com)

Referenced From : Programming Game AI by Example - Chapter 10
*/

#include "GameUtil.h"

class FuzzySet;
class FzSet;
class FuzzyModule;

class FuzzyVariable
{
private:
  
  typedef std::map<std::string, FuzzySet*>  MemberSets;
    
private:
  
  //disallow copies
  FuzzyVariable(const FuzzyVariable&);
  FuzzyVariable& operator=(const FuzzyVariable&);

public:

  FuzzyVariable():m_dMinRange(0.0),m_dMaxRange(0.0){}
  
  //the following methods create instances of the sets named in the method
  //name and add them to the member set map. Each time a set of any type is
  //added the m_dMinRange and m_dMaxRange are adjusted accordingly. All of the
  //methods return a proxy class representing the newly created instance. This
  //proxy set can be used as an operand when creating the rule base.
  FzSet  AddLeftShoulderSet(string name, float minBound, float peak, float maxBound);

  FzSet  AddRightShoulderSet(string name, float minBound, float peak, float maxBound);

  FzSet  AddTriangularSet(string name,
                             float       minBound,
                             float       peak,
                             float       maxBound);

  FzSet  AddSingletonSet(string name,
                            float       minBound,
                            float       peak,
                            float       maxBound);
  
  
  //fuzzify a value by calculating its DOM in each of this variable's subsets
  void        Fuzzify(float val);

  //defuzzify the variable using the max average method
  float       DeFuzzifyMaxAv()const;

  //defuzzify the variable using the centroid method
  float       DeFuzzifyCentroid(int NumSamples)const;

private:
  //a map of the fuzzy sets that comprise this variable
  MemberSets   m_MemberSets;

  //the minimum and maximum value of the range of this variable
  float         m_dMinRange;
  float         m_dMaxRange;
  

  //this method is called with the upper and lower bound of a set each time a
  //new set is added to adjust the upper and lower range values accordingly
  void AdjustRangeToFit(float min, float max);

  //a client retrieves a reference to a fuzzy variable when an instance is
  //created via FuzzyModule::CreateFLV(). To prevent the client from deleting
  //the instance the FuzzyVariable destructor is made private and the 
  //FuzzyModule class made a friend.
  ~FuzzyVariable();

  friend class FuzzyModule;

};

#endif