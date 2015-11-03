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
 *  of the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     General Public License as published by 
 *  the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  The Schulich Delta Central Communication System is distributed 
 *  in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 *  without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     General 
 *  Public License along with the Schulich Delta Central Communication 
 *  System. If not, see <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#pragma once

#include <mbed.h>
#include <VehicleData.h>

class DriverControl
{
public:
   DriverControl(const PinName& deadmanInput,
                 const PinName& hazardsInput,
                 const PinName& rightBlinkerInput,
                 const PinName& leftBlinkerInput,
                 const PinName& brakeInput,
                 const PinName& currentInput,
                 const PinName& regenInput,
                 const PinName& directionInput,
                 const PinName& mpptModeInput,
                 const PinName& arrayOnInput,
                 const PinName& highVoltageInput,
                 VehicleData& vehicleData);

   void initializeDriverControls();
   void readDriverControls();

private:
   void setSpeedAndCurrent();
   float checkIfAboveZeroThreshold(const float input);

   float calculateRunningAverage(const float* averageData);
   void addLatestToRunningAverage(float* averageData, int& currentIndex, float newData);

   void setMovementSpeedAndCurrent(VehicleData::CarDirection direction);
   void setRegenSpeedAndCurrent();

   void zeroDriverInputs();
   void readInputs();

   bool isNewVehicleDirectionInputSafe(VehicleData::CarDirection direction);
   bool isRegenBraking();

private:
   DigitalIn deadmanInput_;
   DigitalIn hazardsInput_;
   DigitalIn rightBlinkerInput_;
   DigitalIn leftBlinkerInput_;
   DigitalIn brakeInput_;
   DigitalIn directionInput_;
   DigitalIn mpptModeInput_;
   DigitalIn arrayOnInput_;
   DigitalIn highVoltageInput_;

   AnalogIn currentInput_;
   AnalogIn regenInput_;

   VehicleData& vehicleData_;

   float runningAverageRegenBrakingData_[5];
   int currentIndexRegenBrakingDataAverage_;
   float runningAverageCurrentData_[5];
   int currentIndexCurrentDataAverage_;
};
