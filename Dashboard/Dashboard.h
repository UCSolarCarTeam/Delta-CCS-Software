/**
 *  Schulich Delta Central Communication System
 *  Made for the OLED displays
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
