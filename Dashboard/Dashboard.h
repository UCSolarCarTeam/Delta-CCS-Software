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
   Dashboard(
      const PinName& mosi,
      const PinName& miso,
      const PinName& clock,
      const PinName& chipSelectChar1Display,
      const PinName& chipSelectChar2Display,
      const PinName& chipSelectChar3Display,
      const VehicleData& vehicleData);

   void initAll();
   void updateDashboard();

private:
   void updateDisplay1();
   void updateDisplay2();
   void updateDisplay3();
   void displayTest(CharDisplay& display);

private:
   SPI serialCommunication_;
   CharDisplay charDisplay1_;
   CharDisplay charDisplay2_;
   CharDisplay charDisplay3_;

   int timer_;

   const VehicleData& vehicleData_;
};
