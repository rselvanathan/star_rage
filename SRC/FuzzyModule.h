#ifndef __FUZZYMODULE_H__
#define __FUZZYMODULE_H__

/*
This class descibes a fuzzy module : a collection of fuzzy variables and the rules that operate them

Author : Mat Buckland (www.ai-junkie.com)

 , pg 438
*/

#include "GameUtil.h"

#include "FuzzySet.h"
#include "FuzzyVariable.h"
#include "FuzzyRule.h"
#include "FuzzyOperators.h"
#include "FzSet.h"
#include "FuzzyHedges.h"

class FuzzyModule
{
public:
	/*
	Must pass one of these values to the defuzzify method. So far only support for centroid and MaxAv
	methods
	*/
	enum DefuzzifyMethod {max_av, centroid};
	/*
	when calculating the centroid of the fuzzy manifold this value is used to determine ho wmany coss sections should
	be sampled
	*/
	enum {NumSamples = 15};

private:
	typedef map<string, FuzzyVariable*> VarMap;

public:
	~FuzzyModule();
	/* Create a new "empty" fuzzy variable and returns a ference to it */
	FuzzyVariable& CreateFLV(const string& Varname);
	/* Add a rule to the module */
	void AddRule(FuzzyTerm& antecedent, FuzzyTerm& consequence);
	/* Method calls the Fuzzify method from the named FLV */
	void Fuzzify(const string& NameOfFLV, float val);
	/* Given a fuzzy Variable and a deffuzification method , this returns a crisp value */
	float Defuzzify(const string& NameOfFLV, DefuzzifyMethod method = max_av);

private:
	/* A Map of all the fuzzy variables */ 
	VarMap m_Variables;
	/* A vector containing all the fuzzy rules */
	vector<FuzzyRule*> m_Rules;
	/* Zeros the DOMS of the consequents of each rule. Used by Defuzzify() */
	void SetConfidencesOfConsequentsToZero();
};

#endif