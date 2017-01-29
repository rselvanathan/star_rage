#include "FuzzyOperators.h"
#include "GameUtil.h"

//--------------------------------FZAND----------------------------------------------

FzAND::FzAND(const FzAND& fa)
{	
   for (vector<FuzzyTerm*>::const_iterator curTerm = fa.m_Terms.begin(); curTerm != fa.m_Terms.end(); ++curTerm)
   {
     m_Terms.push_back((*curTerm)->Clone());
   }
}

  //ctor using two terms
FzAND::FzAND(FuzzyTerm& op1, FuzzyTerm& op2)
{
   m_Terms.push_back(op1.Clone());
   m_Terms.push_back(op2.Clone());
}

//ctor using three terms
FzAND::FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3)
{
   m_Terms.push_back(op1.Clone());
   m_Terms.push_back(op2.Clone());
   m_Terms.push_back(op3.Clone());
}

      //ctor using four terms
FzAND::FzAND(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4)
{
   m_Terms.push_back(op1.Clone());
   m_Terms.push_back(op2.Clone());
   m_Terms.push_back(op3.Clone());
   m_Terms.push_back(op4.Clone());
}

FzAND::~FzAND()
{
  for (std::vector<FuzzyTerm*>::iterator curTerm = m_Terms.begin(); curTerm != m_Terms.end(); ++curTerm)
  {
    SAFE_DELETE(*curTerm);
  }
}

//  the AND operator returns the minimum DOM of the sets it is operating on
float FzAND::GetDOM()const
{
  float smallest = MaxFloat;

  for (vector<FuzzyTerm*>::const_iterator curTerm = m_Terms.begin(); curTerm != m_Terms.end(); ++curTerm)
  {
    if ((*curTerm)->GetDOM() < smallest)
    {
      smallest = (*curTerm)->GetDOM();
    }
  }

  return smallest;
}

void FzAND::ORwithDOM(float val)
{
  for (vector<FuzzyTerm*>::iterator curTerm = m_Terms.begin(); curTerm != m_Terms.end(); ++curTerm)
  {
    (*curTerm)->ORwithDOM(val);
  }
}

void FzAND::ClearDOM()
{
  for (vector<FuzzyTerm*>::iterator curTerm = m_Terms.begin(); curTerm != m_Terms.end(); ++curTerm)
  {
    (*curTerm)->ClearDOM();
  }
}

//-------------------------------------------FZOR------------------------------------------------

FzOR::FzOR(const FzOR& fa)
{
   for (vector<FuzzyTerm*>::const_iterator curTerm = fa.m_Terms.begin(); curTerm != fa.m_Terms.end(); ++curTerm)
   {
     m_Terms.push_back((*curTerm)->Clone());
   }
}

  //ctor using two terms
FzOR::FzOR(FuzzyTerm& op1, FuzzyTerm& op2)
{
   m_Terms.push_back(op1.Clone());
   m_Terms.push_back(op2.Clone());
}

    //ctor using three terms
FzOR::FzOR(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3)
{
   m_Terms.push_back(op1.Clone());
   m_Terms.push_back(op2.Clone());
   m_Terms.push_back(op3.Clone());
}

      //ctor using four terms
FzOR::FzOR(FuzzyTerm& op1, FuzzyTerm& op2, FuzzyTerm& op3, FuzzyTerm& op4)
{
   m_Terms.push_back(op1.Clone());
   m_Terms.push_back(op2.Clone());
   m_Terms.push_back(op3.Clone());
   m_Terms.push_back(op4.Clone());
}

FzOR::~FzOR()
{
  for (vector<FuzzyTerm*>::iterator curTerm = m_Terms.begin(); curTerm != m_Terms.end(); ++curTerm)
  {
    SAFE_DELETE(*curTerm);
  }
}

//  the OR operator returns the maximum DOM of the sets it is operating on
float FzOR::GetDOM()const
{
  float largest = MinFloat;

  for (vector<FuzzyTerm*>::const_iterator curTerm = m_Terms.begin(); curTerm != m_Terms.end(); ++curTerm)
  {
    if ((*curTerm)->GetDOM() > largest)
    {
      largest = (*curTerm)->GetDOM();
    }
  }

  return largest;
}