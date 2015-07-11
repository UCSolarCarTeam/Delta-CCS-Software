/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>
class VehicleData;

class LedBmuErrorOutputService
{
public:
   LedBmuErrorOutputService(VehicleData& vehicleData);

   void updateLeds();

private:
   VehicleData& vehicleData_;

   DigitalOut led1_;
   DigitalOut led2_;
   DigitalOut led3_;
   DigitalOut led4_;
};
