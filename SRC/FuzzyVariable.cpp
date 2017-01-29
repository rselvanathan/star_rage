#include "FuzzyVariable.h"
#include "FuzzyOperators.h"
#include "FuzzySet.h"
#include "FuzzySet_Singleton.h"
#include "FuzzySet_LeftShoulder.h"
#include "FuzzySet_RightShoulder.h"
#include "FuzzySet_Triangle.h"
#include "FzSet.h"
#include "GameUtil.h"

// Destructor
FuzzyVariable::~FuzzyVariable()
{
  for (MemberSets::iterator it = m_MemberSets.begin(); it != m_MemberSets.end(); ++it)
  {
    SAFE_DELETE(it->second);
  }
}

//  takes a crisp value and calculates its degree of membership for each set
//  in the variable.
void FuzzyVariable::Fuzzify(float val)
{    
  //make sure the value is within the bounds of this variable
  assert ( (val >= m_dMinRange) && (val <= m_dMaxRange) && 
           "<FuzzyVariable::Fuzzify>: value out of range");

  //for each set in the flv calculate the DOM for the given value
  for (MemberSets::const_iterator curSet = m_MemberSets.begin(); curSet != m_MemberSets.end(); ++curSet)
  {
    curSet->second->SetDOM(curSet->second->CalculateDOM(val));
  }
}

// defuzzifies the value by averaging the maxima of the sets that have fired
//
// OUTPUT = sum (maxima * DOM) / sum (DOMs) 
float FuzzyVariable::DeFuzzifyMaxAv()const
{
  float bottom = 0.0;
  float top    = 0.0;

  MemberSets::const_iterator curSet;
  for (curSet = m_MemberSets.begin(); curSet != m_MemberSets.end(); ++curSet)
  {
    bottom += curSet->second->GetDOM();

    top += curSet->second->GetRepresentativeVal() * curSet->second->GetDOM();
  }

  //make sure bottom is not equal to zero
  if (closeEnough(0, bottom)) return 0.0;

  return top / bottom;   
}

//  defuzzify the variable using the centroid method
float FuzzyVariable::DeFuzzifyCentroid(int NumSamples)const
{
  //calculate the step size
  float StepSize = (m_dMaxRange - m_dMinRange)/(float)NumSamples;

  float TotalArea    = 0.0;
  float SumOfMoments = 0.0;

  //step through the range of this variable in increments equal to StepSize
  //adding up the contribution (lower of CalculateDOM or the actual DOM of this
  //variable's fuzzified value) for each subset. This gives an approximation of
  //the total area of the fuzzy manifold.(This is similar to how the area under
  //a curve is calculated using calculus... the heights of lots of 'slices' are
  //summed to give the total area.)
  //
  //in addition the moment of each slice is calculated and summed. Dividing
  //the total area by the sum of the moments gives the centroid. (Just like
  //calculating the center of mass of an object)
  for (int samp=1; samp<=NumSamples; ++samp)
  {
    //for each set get the contribution to the area. This is the lower of the 
    //value returned from CalculateDOM or the actual DOM of the fuzzified 
    //value itself   
    MemberSets::const_iterator curSet = m_MemberSets.begin();
    for (curSet; curSet != m_MemberSets.end(); ++curSet)
    {
      float contribution = 
          MinOf(curSet->second->CalculateDOM(m_dMinRange + samp * StepSize),
                curSet->second->GetDOM());

      TotalArea += contribution;

      SumOfMoments += (m_dMinRange + samp * StepSize)  * contribution;
    }
  }

  //make sure total area is not equal to zero
  if (closeEnough(0, TotalArea)) return 0.0;
  
  return (SumOfMoments / TotalArea);
}

//  adds a triangular shaped fuzzy set to the variable
FzSet FuzzyVariable::AddTriangularSet(string name,
									 float       minBound,
                                     float       peak,
                                     float       maxBound)
{
  m_MemberSets[name] = new FuzzySet_Triangle(peak,
                                             peak-minBound,
                                             maxBound-peak);
  //adjust range if necessary
  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}

//  adds a left shoulder type set
FzSet FuzzyVariable::AddLeftShoulderSet(string name,
                                        float       minBound,
                                        float       peak,
                                        float       maxBound)
{
  m_MemberSets[name] = new FuzzySet_LeftShoulder(peak, peak-minBound, maxBound-peak);

  //adjust range if necessary
  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}

//  adds a left shoulder type set
FzSet FuzzyVariable::AddRightShoulderSet(string name,
                                         float       minBound,
                                         float       peak,
                                         float       maxBound)
{
  m_MemberSets[name] = new FuzzySet_RightShoulder(peak, peak-minBound, maxBound-peak);

  //adjust range if necessary
  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}

//  adds a singleton to the variable
FzSet FuzzyVariable::AddSingletonSet(string name,
                                    float       minBound,
                                    float       peak,
                                    float       maxBound)
{
  m_MemberSets[name] = new FuzzySet_Singleton(peak,
                                              peak-minBound,
                                              maxBound-peak);

  AdjustRangeToFit(minBound, maxBound);

  return FzSet(*m_MemberSets[name]);
}

//  this method is called with the upper and lower bound of a set each time a
//  new set is added to adjust the upper and lower range values accordingly
void FuzzyVariable::AdjustRangeToFit(float minBound, float maxBound)
{
  if (minBound < m_dMinRange) m_dMinRange = minBound;
  if (maxBound > m_dMaxRange) m_dMaxRange = maxBound;
}