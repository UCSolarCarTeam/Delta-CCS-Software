/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <MpptData.h>

MpptData::MpptData()
: type(Helianthus)
, mode(Static)
, voltageIn(0.0f)
, voltageOut(0.0f)
, currentIn(0.0f)
, currentOut(0.0f)
{
}
