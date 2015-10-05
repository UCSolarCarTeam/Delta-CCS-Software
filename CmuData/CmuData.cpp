/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <CmuData.h>

int CmuData::cmuCount = 0;

CmuData::CmuData()
{
   pcbTemperature = 46 + (CmuData::cmuCount * 10);
   cellTemperature = 47 + (CmuData::cmuCount * 10);

   for(int i = 0; i < 8; i++)
   {
      cellVoltage[i] = i + 48 + (CmuData::cmuCount * 10);
   }

   CmuData::cmuCount++;
}
