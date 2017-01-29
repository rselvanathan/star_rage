#include "FuzzySet.h"
#include "FuzzyVariable.h"
#include "FuzzyRule.h"
#include "FuzzyOperators.h"
#include "FzSet.h"
#include "FuzzyHedges.h"
#include "FuzzyModule.h"
#include "GameUtil.h"

// Destructor
FuzzyModule::~FuzzyModule()
{
  VarMap::iterator curVar = m_Variables.begin();
  for (curVar; curVar != m_Variables.end(); ++curVar)
  {
    delete curVar->second;
  }

  std::vector<FuzzyRule*>::iterator curRule = m_Rules.begin();
  for (curRule; curRule != m_Rules.end(); ++curRule)
  {
    delete *curRule;
  }
}

//  creates a new fuzzy variable and returns a reference to it
FuzzyVariable& FuzzyModule::CreateFLV(const string& VarName)
{
  m_Variables[VarName] = new FuzzyVariable();;

  return *m_Variables[VarName];
}

// Add a Rule
void FuzzyModule::AddRule(FuzzyTerm& antecedent, FuzzyTerm& consequence)
{
  m_Rules.push_back(new FuzzyRule(antecedent, consequence));
}

//  this method calls the Fuzzify method of the variable with the same name
//  as the key
void FuzzyModule::Fuzzify(const string& NameOfFLV, float val)
{
  //first make sure the key exists
  assert ( (m_Variables.find(NameOfFLV) != m_Variables.end()) &&
          "<FuzzyModule::Fuzzify>:key not found");

  m_Variables[NameOfFLV]->Fuzzify(val);
}

//  given a fuzzy variable and a deffuzification method this returns a 
//  crisp value
float FuzzyModule::Defuzzify(const string& NameOfFLV, DefuzzifyMethod method)
{
	//first make sure the key exists
  assert ( (m_Variables.find(NameOfFLV) != m_Variables.end()) &&
          "<FuzzyModule::DeFuzzifyMaxAv>:key not found");

  //clear the DOMs of all the consequents of all the rules
  SetConfidencesOfConsequentsToZero();

  //process the rules
  std::vector<FuzzyRule*>::iterator curRule = m_Rules.begin();
  for (curRule; curRule != m_Rules.end(); ++curRule)
  {
    (*curRule)->Calculate();
  }

  //now defuzzify the resultant conclusion using the specified method
  switch (method)
  {
  case centroid:

    return m_Variables[NameOfFLV]->DeFuzzifyCentroid(NumSamples);

    break;

  case max_av:

    return m_Variables[NameOfFLV]->DeFuzzifyMaxAv();

    break;
  }

  return 0;
}

//  zeros the DOMs of the consequents of each rule
void FuzzyModule::SetConfidencesOfConsequentsToZero()
{
  for (vector<FuzzyRule*>::iterator curRule = m_Rules.begin(); curRule != m_Rules.end(); ++curRule)
  {
    (*curRule)->SetConfidenceOfConsequentToZero();
  }
}