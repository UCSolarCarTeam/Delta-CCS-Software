/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

// Solar car includes
#include <DriverControl.h>
#include <VehicleData.h>

namespace
{
   // Control parameters
   // Wavesculptor 22 requires velocity in RPM
   const unsigned int MAX_FORWARD_RPM = 1000; //Was 1298 on old system
   const unsigned int MAX_REVERSE_RPM = 200;
   const unsigned int MAX_CURRENT_AMPS = 80;
   //RPM to Kph is wheel diameter * pi * 60 s / 1000 m
   const float RPM_TO_KPH_CONVERSION = 0.545 * 3.14159265358979323 * 60.0 / 1000.0;
   const float MAX_CURRENT = 1.0f;

   const float NON_ZERO_THRESHOLD = 0.05f;
   const float MAX_SPEED_TO_CHANGE_DIRECTIONS = 3.0f;
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
    vehicleData_.pc.printf("Init Driver control\n");
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
      vehicleData_.vehicleVelocityKph > MAX_SPEED_TO_CHANGE_DIRECTIONS)
   {
      //Safety measure. Not allow changing direction at speed. Stop car.
      vehicleData_.driverSetCurrent = 0.0f;
      vehicleData_.driverSetSpeedRpm = 0.0f;
      vehicleData_.driverSetCurrentA = 0.0f;
      vehicleData_.driverSetSpeedKph = 0.0f;
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
      if(vehicleData_.carDirection == VehicleDataEnums::FORWARD)
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
//      vehicleData_.driverSetCurrent = 0.0f;
//      vehicleData_.driverSetSpeedRpm = 0.0f;
//      vehicleData_.driverSetSpeedKph = 0;
//      vehicleData_.driverSetCurrentA = 0.0f;
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
   vehicleData_.driverSetCurrent = calculateRunningAverage(runningAverageCurrentData_);
   vehicleData_.driverSetSpeedKph = vehicleData_.driverSetSpeedRpm * RPM_TO_KPH_CONVERSION;
   vehicleData_.driverSetCurrentA = vehicleData_.driverSetCurrent * MAX_CURRENT_AMPS;
}

void DriverControl::setReverseSpeedAndCurrent()
{
   vehicleData_.driverSetSpeedRpm = -checkIfAboveZeroThreshold(velocityInput_) * MAX_REVERSE_RPM;
   vehicleData_.driverSetCurrent = checkIfAboveZeroThreshold(currentInput_) * MAX_CURRENT;
   vehicleData_.driverSetSpeedKph = -vehicleData_.driverSetSpeedRpm * RPM_TO_KPH_CONVERSION;
   vehicleData_.driverSetCurrentA = vehicleData_.driverSetCurrent * MAX_CURRENT_AMPS;
}
