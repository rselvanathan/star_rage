#include "FuzzySet_Triangle.h"
#include "GameUtil.h"

FuzzySet_Triangle::FuzzySet_Triangle(float mid, float LeftOffset, float RighOffset) :
	FuzzySet(mid), m_fPeakPoint(mid), m_fRightOffset(RighOffset), m_fLeftOffset(LeftOffset)
{}

float FuzzySet_Triangle::CalculateDOM(float val)const
{
  //test for the case where the triangle's left or right offsets are zero
  //(to prevent divide by zero errors below)
  if ( (closeEnough(m_fRightOffset, 0.0) && (closeEnough(m_fPeakPoint, val))) ||
       (closeEnough(m_fLeftOffset, 0.0) && (closeEnough(m_fPeakPoint, val))) )
  {
    return 1.0;
  }

  //find DOM if left of center
  if ( (val <= m_fPeakPoint) && (val >= (m_fPeakPoint - m_fLeftOffset)) )
  {
    double grad = 1.0 / m_fLeftOffset;

    return grad * (val - (m_fPeakPoint - m_fLeftOffset));
  }
  //find DOM if right of center
  else if ( (val > m_fPeakPoint) && (val < (m_fPeakPoint + m_fRightOffset)) )
  {
    double grad = 1.0 / -m_fRightOffset;

    return grad * (val - m_fPeakPoint) + 1.0;
  }
  //out of range of this FLV, return zero
  else
  {
    return 0.0;
  }
}