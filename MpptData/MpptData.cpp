/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <MpptData.h>

int MpptData::mpptCount = 0;

MpptData::MpptData()
: type(Helianthus)
, mode(Static)
, voltageIn(126.0f + MpptData::mpptCount * 4)
, voltageOut(127.0f + MpptData::mpptCount * 4)
, currentIn(128.0f + MpptData::mpptCount * 4)
, currentOut(129.0f + MpptData::mpptCount * 4)
{
	MpptData::mpptCount++;
}
