#pragma once
/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

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
