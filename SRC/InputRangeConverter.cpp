/*
A class that will conver the raw input range claues to sensitivity calibrated range values.

Author : Romesh Selvanathan

Date : Oct 2012
*/

#include "InputRangeConverter.h"
#include "FileIO.h"

//------------------------------------------------------------------------------------------------------
// Constructor that will read in the range values as well as the sensitivty values in order to 
// do the coversions
//------------------------------------------------------------------------------------------------------
InputRangeConverter::InputRangeConverter(std::wifstream& file)
{
	if(!file) 
		throw std::exception("Invalid file provided to InputRangeConverter constructor");

	unsigned num_conversions = AttemptRead<unsigned>(file);
	for(unsigned i = 0; i < num_conversions; ++i)
	{
		Range range;
		Converter converter;

		range = static_cast<Range>(AttemptRead<unsigned>(file));
		converter.m_minInput = AttemptRead<double>(file);
		converter.m_maxInput = AttemptRead<double>(file);
		converter.m_minOutput = AttemptRead<double>(file);
		converter.m_maxOutput = AttemptRead<double>(file);

		if((converter.m_maxInput < converter.m_minInput) || (converter.m_maxOutput < converter.m_minOutput))
			throw std::exception("Invalid input range conversion");

		m_conversionMap.insert(std::make_pair(range, converter));
	}

	file.close();
}