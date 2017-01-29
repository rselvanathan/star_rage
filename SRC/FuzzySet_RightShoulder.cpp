#include "GameUtil.h"
#include "FuzzySet_RightShoulder.h"

/* Contructor */
FuzzySet_RightShoulder::FuzzySet_RightShoulder(float peak, float LeftOffset, float RightOffset) :
	FuzzySet(((peak + RightOffset) + peak) / 2), m_fPeakPoint(peak), m_fRightOffset(RightOffset), m_fLeftOffset(LeftOffset)
{}

float FuzzySet_RightShoulder::CalculateDOM(float val)const
{
  //test for the case where the left or right offsets are zero
  //(to prevent divide by zero errors below)
  if ( (closeEnough(m_fRightOffset, 0.0) && (closeEnough(m_fPeakPoint, val))) ||
       (closeEnough(m_fLeftOffset, 0.0) && (closeEnough(m_fPeakPoint, val))) )
  {
    return 1.0;
  }
  
  //find DOM if left of center
  else if ( (val <= m_fPeakPoint) && (val > (m_fPeakPoint - m_fLeftOffset)) )
  {
    double grad = 1.0 / m_fLeftOffset;

    return grad * (val - (m_fPeakPoint - m_fLeftOffset));
  }
  //find DOM if right of center and less than center + right offset
  else if ( (val > m_fPeakPoint) && (val <= m_fPeakPoint+m_fRightOffset) )
  {
    return 1.0;
  }

  else
  {
    return 0;
  }
} 