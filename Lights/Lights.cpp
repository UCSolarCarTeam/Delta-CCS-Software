/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

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

Lights::Lights(PinName headLightsPin, 
               PinName brakeLightsPin, 
               PinName leftBlinkerPin, 
               PinName rightBlinkerPin, 
               VehicleData& vehicleData)
: headLightsPin_(headLightsPin)
, brakeLightsPin_(brakeLightsPin)
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
   vehicleData_.headlightOn = false;
   vehicleData_.brakelightOn = false;
   vehicleData_.hazardsActivated = false;

   updateLights();
}

void Lights::updateLights()
{
   updateBlinkersAndHazard();

   setLight(headLightsPin_, vehicleData_.headlightOn);
   setLight(brakeLightsPin_, vehicleData_.brakelightOn);
}

void Lights::updateBlinkersAndHazard()
{
   if(timer_.read_ms() >= BLINK_TIME)
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
