/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <CmuData.h>

CmuData::CmuData()
{
   pcbTemperature = 46;
   cellTemperature = 47;

   for(int i = 0; i < 8; i++)
   {
      cellVoltage[i] = i + 48;
   }
}
