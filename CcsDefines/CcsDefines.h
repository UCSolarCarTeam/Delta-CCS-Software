#pragma once

/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2015 by University of Calgary Solar Car Team
-------------------------------------------------------*/

namespace CcsDefines
{
   // Control parameters
   // Wavesculptor 22 requires velocity in RPM
   const int MAX_FORWARD_RPM = 1000; //Was 1298 on old system
   const int MAX_REVERSE_RPM = -200;
   const float MAX_BATTERY_CURRENT_DRAIN = -100.0f;
   const float MAX_BATTERY_CURRENT_CHARGE = 60.0f;
   const float MAX_CURRENT_AMPS = 70.0;
   const float REGEN_INPUT_SCALING = 0.5f;
   //RPM to Kph is wheel diameter * pi * 60 s / 1000 m
   const float RPM_TO_KPH_CONVERSION = 0.545 * 3.14159265358979323 * 60.0 / 1000.0;
   const float RPM_TO_MPS_CONVERSION = 0.545 * 3.14159265358979323 / 60.0;
   const float MPS_KPH_CONVERSION = 3.6;
   // These number masks out all the errors that don't really matter
   const unsigned int MAJOR_BMU_ERROR_MASK = 0x1C37;
   const unsigned int MAJOR_MC_ERROR_MASK = 0xFF;
}
