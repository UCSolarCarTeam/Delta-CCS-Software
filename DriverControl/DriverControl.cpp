/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <CcsDefines.h>
#include <DriverControl.h>

using namespace CcsDefines;

namespace
{
   const float NON_ZERO_THRESHOLD = 0.05f;
   const float MAX_SPEED_TO_CHANGE_DIRECTIONS = 1.0f; // 1 m/s or 3.6 km/h
}

DriverControl::DriverControl(const PinName& deadmanInput,
                             const PinName& hazardsInput,
                             const PinName& rightBlinkerInput,
                             const PinName& leftBlinkerInput,
                             const PinName& brakeInput,
                             const PinName& currentInput,
                             const PinName& regenInput,
                             const PinName& directionInput,
                             const PinName& mpptModeInput,
                             VehicleData& vehicleData)
: deadmanInput_(deadmanInput)
, hazardsInput_(hazardsInput)
, rightBlinkerInput_(rightBlinkerInput)
, leftBlinkerInput_(leftBlinkerInput)
, brakeInput_(brakeInput)
, directionInput_(directionInput)
, mpptModeInput_(mpptModeInput)
, currentInput_(currentInput)
, regenInput_(regenInput)
, vehicleData_(vehicleData)
, currentIndexRegenBrakingDataAverage_(0)
, currentIndexCurrentDataAverage_(0)
{
   for(int i = 0; i < 5; i++)
   {
      runningAverageRegenBrakingData_[i] = 0.0f;
      runningAverageCurrentData_[i] = 0.0f;
   }
}

void DriverControl::initializeDriverControls()
{
   readDriverControls();
}

void DriverControl::readDriverControls()
{
   readInputs();
   setSpeedAndCurrent();
}

void DriverControl::setSpeedAndCurrent()
{
   // This is read only once per loop because it could change due to noise if read again
   // after the safety check.
   const VehicleData::CarDirection directionInput =
      static_cast<VehicleData::CarDirection>(directionInput_.read());

   if (vehicleData_.deadmanPressed &&
      isNewVehicleDirectionInputSafe(directionInput))
   {
      if (isRegenBraking())
      {
         setRegenSpeedAndCurrent();
      }
      else
      {
         setMovementSpeedAndCurrent(directionInput);
      }
   }
   else
   {
      zeroDriverInputs();
   }
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

   return total / 5.0f;
}

void DriverControl::addLatestToRunningAverage(
   float* averageData, int& currentIndex, float newData)
{
   averageData[currentIndex] = newData;
   currentIndex++;
   if(currentIndex >= 5)
   {
      currentIndex = 0;
   }
}

void DriverControl::setMovementSpeedAndCurrent(VehicleData::CarDirection direction)
{
   vehicleData_.carDirection = direction;
   if(vehicleData_.carDirection == VehicleData::Forward)
   {
      vehicleData_.driverSetSpeedRpm = MAX_FORWARD_RPM;
   }
   else
   {
      vehicleData_.driverSetSpeedRpm = MAX_REVERSE_RPM;
   }
   vehicleData_.driverSetCurrentPercentage = calculateRunningAverage(runningAverageCurrentData_);
   vehicleData_.driverSetCurrent = vehicleData_.driverSetCurrentPercentage * MAX_CURRENT_AMPS;
}

void DriverControl::setRegenSpeedAndCurrent()
{
   vehicleData_.driverSetSpeedRpm = 0.0f;
   vehicleData_.driverSetCurrentPercentage =
      calculateRunningAverage(runningAverageRegenBrakingData_);
   vehicleData_.driverSetCurrent =
      vehicleData_.driverSetCurrentPercentage * MAX_REGEN_CURRENT_AMPS;
}

void DriverControl::zeroDriverInputs()
{
   vehicleData_.driverSetCurrentPercentage = 0.0f;
   vehicleData_.driverSetSpeedRpm = 0.0f;
   vehicleData_.driverSetCurrent = 0.0f;
}

void DriverControl::readInputs()
{
   vehicleData_.deadmanPressed = deadmanInput_;
   vehicleData_.hazardsActivated = hazardsInput_;
   vehicleData_.rightBlinkerActivated = rightBlinkerInput_;
   vehicleData_.leftBlinkerActivated = leftBlinkerInput_;
   vehicleData_.brakelightOn = brakeInput_;
   vehicleData_.dynamicModeActivated = mpptModeInput_;

   addLatestToRunningAverage(runningAverageRegenBrakingData_, currentIndexRegenBrakingDataAverage_,
      checkIfAboveZeroThreshold(regenInput_));
   addLatestToRunningAverage(runningAverageCurrentData_, currentIndexCurrentDataAverage_,
      checkIfAboveZeroThreshold(currentInput_));
}

bool DriverControl::isNewVehicleDirectionInputSafe(VehicleData::CarDirection direction)
{
   return vehicleData_.carDirection == direction ||
      vehicleData_.vehicleVelocity <= MAX_SPEED_TO_CHANGE_DIRECTIONS;
}

bool DriverControl::isRegenBraking()
{
   return regenInput_ > NON_ZERO_THRESHOLD;
}
