/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <MpptData.h>

MpptData::MpptData()
: type(Helianthus)
, mode(Static)
, voltageIn(56.0f)
, voltageOut(57.0f)
, currentIn(58.0f)
, currentOut(59.0f)
{
}
