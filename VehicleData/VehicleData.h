#pragma once
/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>
#include <CmuData.h>
#include <MpptData.h>

namespace VehicleDataDefines
{
   const int NUMBER_OF_MPPTS = 7;
}

// Container for all parameters within the CCS,
struct VehicleData
{
   VehicleData();

   enum CarDirection {Forward = 0, Reverse = 1};

   enum PrechargeStates
   {
      Error = 0,
      Idle = 1,
      EnablePack = 5,
      Measure = 2,
      Precharge = 3,
      Run = 4,
   };

   //Driver inputs
   float reportedMotorCurrent;
   float driverSetCurrent;
   float driverSetCurrentPercentage;
   float driverSetSpeedRpm;
   bool deadmanPressed;
   VehicleData::CarDirection carDirection;

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
   unsigned char motorOneReceivedErrorCount;
   unsigned char motorOneTransmittedErrorCount;
   short int motorOneActiveMotor;
   short int motorOneErrorFlags;
   short int motorOneLimitFlags;

   unsigned char motorTwoReceivedErrorCount;
   unsigned char motorTwoTransmittedErrorCount;
   short int motorTwoActiveMotor;
   short int motorTwoErrorFlags;
   short int motorTwoLimitFlags;

   //BMU Data
   CmuData cmuData[4];
   float packStateOfCharge;
   float packStateOfChargePercentage;
   float balancePackStateOfCharge;
   float balancePackStateOfChargePercentage;
   unsigned char prechargeDriverStatusFlags;
   PrechargeStates prechargeState;
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

   // MPPT
   MpptData mpptData[VehicleDataDefines::NUMBER_OF_MPPTS];
   bool dynamicModeActivated;

   // Accessory power unit
   bool secondaryBatteryUnderVoltage;

   //Light fields
   bool leftBlinkerActivated;
   bool rightBlinkerActivated;
   bool brakelightOn;
   bool hazardsActivated;

   // Power on inputs
   bool arrayActivated;
   bool highVoltageActivated;
   bool overcurrentProtectionTripped;

   Serial pc;
};
