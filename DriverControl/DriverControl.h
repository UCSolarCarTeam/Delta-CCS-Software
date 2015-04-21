/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

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
                 VehicleData& vehicleData);

   void initializeDriverControls();
   void readDriverControls();

private:
   void setSpeedAndCurrent();
   float checkIfAboveZeroThreshold(const float input);

   float calculateRunningAverage(const float* averageData);
   void addLatestToRunningAverage(float* averageData, int& currentIndex, float newData);

   void setMovementSpeedAndCurrent(VehicleDataEnums::CarDirection direction);
   void setRegenSpeedAndCurrent();

   void zeroDriverInputs();
   void readInputs();

   bool isNewVehicleDirectionInputSafe(VehicleDataEnums::CarDirection direction);
   bool isRegenBraking();

private:
   DigitalIn deadmanInput_;
   DigitalIn hazardsInput_;
   DigitalIn rightBlinkerInput_;
   DigitalIn leftBlinkerInput_;
   DigitalIn brakeInput_;
   DigitalIn directionInput_;

   AnalogIn currentInput_;
   AnalogIn regenInput_;

   VehicleData& vehicleData_;

   float runningAverageRegenBrakingData_[5];
   int currentIndexRegenBrakingDataAverage_;
   float runningAverageCurrentData_[5];
   int currentIndexCurrentDataAverage_;
};
