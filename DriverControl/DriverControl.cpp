/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <CcsDefines.h>
#include <DriverControl.h>
#include <VehicleData.h>

using namespace CcsDefines;

namespace
{
   const float MAX_CURRENT = 1.0f;

   const float NON_ZERO_THRESHOLD = 0.05f;
   const float MAX_SPEED_TO_CHANGE_DIRECTIONS = 1.0f; // 1 m/s or 3.6 km/h
}

DriverControl::DriverControl(PinName deadmanInput,
                             PinName hazardsInput,
                             PinName rightBlinkerInput,
                             PinName leftBlinkerInput,
                             PinName brakeInput,
                             PinName currentInput,
                             PinName velocityInput,
                             PinName directionInput,
                             VehicleData& vehicleData)
: deadmanInput_(deadmanInput)
, hazardsInput_(hazardsInput)
, rightBlinkerInput_(rightBlinkerInput)
, leftBlinkerInput_(leftBlinkerInput)
, brakeInput_(brakeInput)
, directionInput_(directionInput)
, currentInput_(currentInput)
, velocityInput_(velocityInput)
, vehicleData_(vehicleData)
, currentIndexRpmDataAverage_(0)
, currentIndexCurrentDataAverage_(0)
, currentIndexCarDirectionAverage_(0)
{
   for(int i = 0; i < 5; i++)
   {
      runningAverageRpmData_[i] = 0.0f;
      runningAverageCurrentData_[i] = 0.0f;
      runningAverageCarDirectionData_[i] = 0;
   }
}

void DriverControl::initializeDriverControls()
{
   readDriverControls();
}

void DriverControl::readDriverControls()
{
   vehicleData_.deadmanPressed = deadmanInput_;
   vehicleData_.hazardsActivated = hazardsInput_;
   vehicleData_.rightBlinkerActivated = rightBlinkerInput_;
   vehicleData_.leftBlinkerActivated = leftBlinkerInput_;
   vehicleData_.brakelightOn = brakeInput_;
   vehicleData_.hazardsActivated = hazardsInput_;

   const int directionInput = directionInput_;

   if(vehicleData_.carDirection != directionInput &&
      vehicleData_.vehicleVelocity > MAX_SPEED_TO_CHANGE_DIRECTIONS)
   {
      //Safety measure. Not allow changing direction at speed. Stop car.
      vehicleData_.driverSetCurrentPercentage = 0.0f;
      vehicleData_.driverSetSpeedRpm = 0.0f;
      vehicleData_.driverSetCurrent = 0.0f;
   }
   else
   {
      vehicleData_.carDirection = static_cast<VehicleDataEnums::CarDirection>(directionInput);

      setSpeedAndCurrent();
   }
}

void DriverControl::setSpeedAndCurrent()
{
 // removed until deadmans are physically implemented
 //  if(vehicleData_.deadmanPressed)
//   {
      if(vehicleData_.carDirection == VehicleDataEnums::Forward)
      {
         setForwardSpeedAndCurrent();
      }
      else
      {
         setReverseSpeedAndCurrent();
      }
//   }
//   else
//   {
//      vehicleData_.driverSetCurrentPercentage = 0.0f;
//      vehicleData_.driverSetSpeedRpm = 0.0f;
//      vehicleData_.driverSetCurrent = 0.0f;
//   }
}

float DriverControl::checkIfAboveZeroThreshold(const float input)
{
   if(input >= NON_ZERO_THRESHOLD)
   {
      return input;
   }
   else
   {
      return 0.0f;
   }
}

float DriverControl::calculateRunningAverage(const float* averageData)
{
   float total = 0.0;
   for(int i = 0; i < 5; i++)
   {
       total += averageData[i];
   }

   return total / 5.0;
}

void DriverControl::addLatestToRunningAverage(float* averageData, int& currentIndex, float newData)
{
   averageData[currentIndex] = newData;
   currentIndex++;
   if(currentIndex >= 5)
   {
      currentIndex = 0;
   }
}

void DriverControl::setForwardSpeedAndCurrent()
{
   addLatestToRunningAverage(runningAverageRpmData_, currentIndexRpmDataAverage_, checkIfAboveZeroThreshold(velocityInput_) * MAX_FORWARD_RPM);
   addLatestToRunningAverage(runningAverageCurrentData_, currentIndexCurrentDataAverage_, checkIfAboveZeroThreshold(currentInput_) * MAX_CURRENT);
   vehicleData_.driverSetSpeedRpm = calculateRunningAverage(runningAverageRpmData_);
   vehicleData_.driverSetCurrentPercentage = calculateRunningAverage(runningAverageCurrentData_);
   vehicleData_.driverSetCurrent = vehicleData_.driverSetCurrentPercentage * MAX_CURRENT_AMPS;
}

void DriverControl::setReverseSpeedAndCurrent()
{
   vehicleData_.driverSetSpeedRpm = -checkIfAboveZeroThreshold(velocityInput_) * MAX_REVERSE_RPM;
   vehicleData_.driverSetCurrentPercentage = checkIfAboveZeroThreshold(currentInput_) * MAX_CURRENT;
   vehicleData_.driverSetCurrent = vehicleData_.driverSetCurrentPercentage * MAX_CURRENT_AMPS;
}
