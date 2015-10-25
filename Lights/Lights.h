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

#include <mbed.h>
class VehicleData;

class Lights
{
public:
   Lights(PinName brakeLightsPin,
          PinName leftBlinkerPin,
          PinName rightBlinker,
          VehicleData& vehicleData);

   void initalizeLights();
   void updateLights();

private:
   void updateBlinkersAndHazard();
   void toggleLight(DigitalOut& pin, bool& lightStatus);
   void setLight(DigitalOut& pin, unsigned int stateIsOn);

private:
   DigitalOut brakeLightsPin_;
   DigitalOut leftBlinkerPin_;
   DigitalOut rightBlinkerPin_;

   //These need their own status since you can't read from digital out for a non LEDX pin
   bool leftBlinkerStatus_;
   bool rightBlinkerStatus_;

   Timer timer_;

   VehicleData& vehicleData_;
};
