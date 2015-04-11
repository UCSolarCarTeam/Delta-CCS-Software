/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#pragma once

//Mbed includes
#include <mbed.h>

//Solar car includes
class VehicleData;

class DriverControl
{
public:
   DriverControl(PinName deadmanInput,
                 PinName hazardsInput,
                 PinName rightBlinkerInput,
                 PinName leftBlinkerInput,
                 PinName brakeInput,
                 PinName currentInput,
                 PinName velocityInput,
                 PinName directionInput,
                 VehicleData& vehicleData);

   void initializeDriverControls();
   void readDriverControls();

private:
   void setSpeedAndCurrent();
   float checkIfAboveZeroThreshold(const float input);

   float calculateRunningAverage(const float* averageData);
   void addLatestToRunningAverage(float* averageData, int& currentIndex, float newData);

   void setForwardSpeedAndCurrent();
   void setReverseSpeedAndCurrent();

private:
   DigitalIn deadmanInput_;
   DigitalIn hazardsInput_;
   DigitalIn rightBlinkerInput_;
   DigitalIn leftBlinkerInput_;
   DigitalIn brakeInput_;
   DigitalIn directionInput_;

   AnalogIn currentInput_;
   AnalogIn velocityInput_;

   VehicleData& vehicleData_;

   float runningAverageRpmData_[5];
   int currentIndexRpmDataAverage_;
   float runningAverageCurrentData_[5];
   int currentIndexCurrentDataAverage_;
   int runningAverageCarDirectionData_[5];
   int currentIndexCarDirectionAverage_;
};
