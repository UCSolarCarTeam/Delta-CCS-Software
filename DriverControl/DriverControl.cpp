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
                             const PinName& arrayOnInput,
                             const PinName& highVoltageInput,
                             VehicleData& vehicleData)
: deadmanInput_(deadmanInput)
, hazardsInput_(hazardsInput)
, rightBlinkerInput_(rightBlinkerInput)
, leftBlinkerInput_(leftBlinkerInput)
, brakeInput_(brakeInput)
, directionInput_(directionInput)
, mpptModeInput_(mpptModeInput)
, arrayOnInput_(arrayOnInput)
, highVoltageInput_(highVoltageInput)
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

   if ( // vehicleData_.deadmanPressed &&
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
      calculateRunningAverage(runningAverageRegenBrakingData_) * REGEN_INPUT_SCALING;
   vehicleData_.driverSetCurrent =
      vehicleData_.driverSetCurrentPercentage * MAX_CURRENT_AMPS;
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
   vehicleData_.brakelightOn = (brakeInput_ == 0); // Active low.
   vehicleData_.dynamicModeActivated = mpptModeInput_;
   vehicleData_.arrayActivated = arrayOnInput_;
   vehicleData_.highVoltageActivated = highVoltageInput_;

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
