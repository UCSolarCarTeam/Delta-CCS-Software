#pragma once
/*-------------------------------------------------------
   Made for the dashboard OLED displays
   By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>

#include <CharDisplay.h>
class VehicleData;

class Dashboard
{
public:
   Dashboard(PinName mosi, PinName miso, PinName clock,
      PinName chipSelectCharDisplay, VehicleData& vehicleData);

   void initAll();
   void displayTest();
   void displayDashboard();

private:
   SPI serialCommunication_;
   CharDisplay charDisplay_;

   VehicleData& vehicleData_;
};
