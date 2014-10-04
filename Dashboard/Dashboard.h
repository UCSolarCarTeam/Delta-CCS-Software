/*-------------------------------------------------------
  Made for the dashboard OLED displays
  By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
  Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

#ifndef DASHBOARD_H
#define DASHBOARD_H

//Mbed includes
#include <mbed.h>

//Solar Car includes
#include <GraphicImageData.h>
#include <CharDisplay.h>
#include <GraphicDisplay.h>

class VehicleData;

class Dashboard
{
public:
  Dashboard(PinName mosi, PinName miso, PinName clock, PinName chipSelectCharDisplay,
            PinName chipSelectGraphicDisplay, PinName graphicDisplayDataSignal,
            PinName graphicDisplayReset,
            VehicleData& vehicleData);
  
  void initAll();
  void displayTest();
  void displayDashboard(); 

private:
  SPI serialCommunication_;
  GraphicDisplay graphic_;
  CharDisplay charDisplay_;
  GraphicImageData graphicImageData_;

  VehicleData& vehicleData_;

  const unsigned char* findDigitNumber(int number);
};

#endif 