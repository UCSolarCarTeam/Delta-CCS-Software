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

#include <mbed.h>

#include <Lights.h>
#include <VehicleData.h>

namespace
{
   unsigned int BLINK_TIME = 500;

   //Change this if PMOS or NMOS
   unsigned int LIGHT_ON = 1;
   unsigned int LIGHT_OFF = 0;
}

Lights::Lights(PinName brakeLightsPin,
               PinName leftBlinkerPin,
               PinName rightBlinkerPin,
               VehicleData& vehicleData)
: brakeLightsPin_(brakeLightsPin)
, leftBlinkerPin_(leftBlinkerPin)
, rightBlinkerPin_(rightBlinkerPin)
, leftBlinkerStatus_(LIGHT_OFF)
, rightBlinkerStatus_(LIGHT_OFF)
, timer_()
, vehicleData_(vehicleData)
{
}

void Lights::initalizeLights()
{
   timer_.start();
   vehicleData_.leftBlinkerActivated = false;
   vehicleData_.rightBlinkerActivated = false;
   vehicleData_.brakelightOn = false;
   vehicleData_.hazardsActivated = false;

   updateLights();
}

void Lights::updateLights()
{
   updateBlinkersAndHazard();

   setLight(brakeLightsPin_, vehicleData_.brakelightOn);
}

void Lights::updateBlinkersAndHazard()
{
   if(static_cast<unsigned int>(timer_.read_ms()) >= BLINK_TIME)
   {
      if(vehicleData_.hazardsActivated)
      {
         //Make sure that they are synced
         leftBlinkerStatus_ = rightBlinkerStatus_;;
         toggleLight(leftBlinkerPin_, leftBlinkerStatus_);
         toggleLight(rightBlinkerPin_, rightBlinkerStatus_);
      }
      else if(vehicleData_.rightBlinkerActivated)
      {
         toggleLight(rightBlinkerPin_, rightBlinkerStatus_);
      }
      else if(vehicleData_.leftBlinkerActivated)
      {
         toggleLight(leftBlinkerPin_, leftBlinkerStatus_);
      }
      else
      {
         setLight(leftBlinkerPin_, LIGHT_OFF);
         setLight(rightBlinkerPin_, LIGHT_OFF);
      }

      timer_.reset();
   }
}

void Lights::toggleLight(DigitalOut& pin, bool& lightStatus)
{
   lightStatus = !lightStatus;
   pin = lightStatus;
}

void Lights::setLight(DigitalOut& pin, unsigned int stateIsOn)
{
   if(stateIsOn)
   {
      pin = LIGHT_ON;
   }
   else
   {
      pin = LIGHT_OFF;
   }
}
