/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

struct CmuData
{
   CmuData();

   short int pcbTemperature;
   short int cellTemperature;
   short int cellVoltage[8];
};
