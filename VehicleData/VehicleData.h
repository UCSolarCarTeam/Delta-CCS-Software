#pragma once
/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>
#include <CmuData.h>

namespace VehicleDataEnums
{
    enum CarDirection {Forward = 0, Reverse = 1};
}

// Container for all parameters within the CCS,
struct VehicleData
{
   VehicleData();

   float reportedMotorCurrent;
   float driverSetCurrent;

   //Driver inputs
   float driverSetCurrentPercentage;
   float driverSetSpeedRpm;
   bool deadmanPressed;
   VehicleDataEnums::CarDirection carDirection;

   //Motor Controller
   float busCurrent;
   float busVoltage;
   float vehicleVelocity;
   float motorVelocityRpm;
   float phaseCCurrent;
   float phaseBCurrent;
   float motorVoltageReal;
   float motorVoltageImaginary;
   float motorCurrentReal;
   float motorCurrentImaginary;
   float backEmfImaginary;
   float ipmHeatSinkTemp;
   float motorTemp;
   float dspBoardTemp;
   float dcBusAmpHours;
   float odometer;
   unsigned char receivedErrorCount;
   unsigned char transmittedErrorCount;
   short int activeMotor;
   short int errorFlags;
   short int limitFlags;

   //BMU Data
   CmuData cmuData[4];
   float packStateOfCharge;
   float packStateOfChargePercentage;
   float balancePackStateOfCharge;
   float balancePackStateOfChargePercentage;
   unsigned char prechargeDriverStatusFlags;
   unsigned char prechargeState;
   unsigned int contactorSupplyVoltage;
   unsigned int prechargeTimerElapsedFlag;
   unsigned int prechargeTimerCounter;
   float batteryVoltage;
   float batteryCurrent;
   unsigned int batteryVoltageThresholdRising;
   unsigned int batteryVoltageThresholdFalling;
   unsigned long bmuStatusFlagsExtended;
   unsigned int fanSpeed0;
   unsigned int fanSpeed1;
   unsigned int fanCurrentConsumption;
   unsigned int cmuCurrentConsumption;

   // Accessory power unit
   bool secondaryBatteryUnderVoltage;

   //Light fields
   bool leftBlinkerActivated;
   bool rightBlinkerActivated;
   bool brakelightOn;
   bool hazardsActivated;

   Serial pc;
};
