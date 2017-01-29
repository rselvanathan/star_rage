#include "GameUtil.h"
#include "FuzzySet_Singleton.h"

FuzzySet_Singleton::FuzzySet_Singleton(float mid, float LeftOffset, float RighOffset) :
	FuzzySet(mid), m_fMidPoint(mid), m_fLeftOffset(LeftOffset), m_fRightOffset(RighOffset)
{}

float FuzzySet_Singleton::CalculateDOM(float val)const
{
  if ( (val >= m_fMidPoint-m_fLeftOffset) &&
       (val <= m_fMidPoint+m_fRightOffset) )
  {
    return 1.0;
  }

  //out of range of this FLV, return zero
  else
  {
    return 0.0;
  }
}