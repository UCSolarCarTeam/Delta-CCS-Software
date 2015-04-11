#pragma once

/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2015 by University of Calgary Solar Car Team
-------------------------------------------------------*/

namespace CcsDefines
{
   // Control parameters
   // Wavesculptor 22 requires velocity in RPM
   const unsigned int MAX_FORWARD_RPM = 1000; //Was 1298 on old system
   const unsigned int MAX_REVERSE_RPM = 200;
   const unsigned int MAX_CURRENT_AMPS = 80;
   //RPM to Kph is wheel diameter * pi * 60 s / 1000 m
   const float RPM_TO_KPH_CONVERSION = 0.545 * 3.14159265358979323 * 60.0 / 1000.0;
   const float RPM_TO_MPS_CONVERSION = 0.545 * 3.14159265358979323 / 60.0;
   const float MPS_KPH_CONVERSION = 3.6;
}
