/*
A class that will conver the raw input range claues to sensitivity calibrated range values.

Author : Romesh Selvanathan

Date : Oct 2012
*/

#include "GameUtil.h"
#include "InputConstants.h"

#ifndef __INPUTRANGECONVERTER_H__
#define __INPUTRANGECONVERTER_H__

class InputRangeConverter
{
private:
	// Internal Helper structure that will help to convert the ranges
	struct Converter
	{
		double m_minInput;
		double m_maxInput;

		double m_minOutput;
		double m_maxOutput;

		template <typename RangeType>
		RangeType Convert(RangeType val) const
		{
			double v = static_cast<double>(val);
			if(v < m_minInput)
				v = m_minInput;
			else if (v > m_maxInput)
				v = m_maxInput;

			double interpolationfactor = (v - m_minInput)/ (m_maxInput - m_minInput);

			return static_cast<RangeType>((interpolationfactor * (m_maxOutput - m_minOutput)) + m_minOutput);
		}
	};

	// Types for shortcut calling
	typedef std::map<Range, Converter> ConversionMapT;

private:
	ConversionMapT m_conversionMap;					// A map that will contain all the conversions

public:
	// Constructor
	explicit InputRangeConverter(std::wifstream& infile);

	// This method is the conversion interface
	template <typename RangeType>
	RangeType Convert(Range range, RangeType val) const
	{
		// Try and find the range and if found, convert the value with the sensitivity values.
		ConversionMapT::const_iterator it = m_conversionMap.find(range);
		if(it == m_conversionMap.end())
			return val;

		return it->second.Convert<RangeType>(val);
	}
};

#endif