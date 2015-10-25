/**
 *  Schulich Delta Central Communication System
 *  Made for the Mbed LPC-1768
 *  By Jordan Heinrichs for the University of Calgary Solar Car Team
 *
 *  Copyright (C) 2015 University of Calgary Solar Car Team
 *
 *  This file is part of the Schulich Delta Central Communication System
 *
 *  The Schulich Delta Central Communication System is free software: 
 *  you can redistribute it and/or modify it under the terms 
 *  of the GNU Affero General Public License as published by 
 *  the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  The Schulich Delta Central Communication System is distributed 
 *  in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 *  without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero 
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General 
 *  Public License along with the Schulich Delta Central Communication 
 *  System. If not, see <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#pragma once

namespace CcsDefines
{
   // Control parameters
   // Wavesculptor 22 requires velocity in RPM
   const int MAX_FORWARD_RPM = 1000; //Was 1298 on old system
   const int MAX_REVERSE_RPM = -200;
   const float MAX_BATTERY_CURRENT_DRAIN = -99.0f;
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
