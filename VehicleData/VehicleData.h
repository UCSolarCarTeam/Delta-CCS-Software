/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

#pragma once

#include <mbed.h>
#include <CmuData.h>

namespace VehicleDataEnums
{
    enum CarDirection {FORWARD = 0, REVERSE = 1};
}

// Container for all parameters within the CCS, 
// This was a conscience decision to not to have good encapsulation here
// for overall ease of writing.
struct VehicleData
{
   VehicleData();

   float actualCurrentA;
   float driverSetCurrentA;

   //Driver inputs
   float actualSpeedRpm;
   float actualCurrent;
   float driverSetCurrent;
   float driverSetSpeedRpm;
   float driverSetSpeedKph;
   bool deadmanPressed;
   VehicleDataEnums::CarDirection carDirection; 

   //Motor Controller
   float busCurrentA;
   float busVoltage;
   float vehicleVelocityKph;
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
   unsigned char prechargeDriverStatusFlags; //Note: change this out for a enum
   unsigned char prechargeState;
   unsigned int contactorSupplyVoltage;
   unsigned prechargeTimerElapsedFlag;
   unsigned prechargeTimerCounter;
   unsigned long batteryVoltage; //V
   long batteryCurrent; //mA
   unsigned int batteryVoltageThresholdRising;
   unsigned int batteryVoltageThresholdFalling;
   unsigned long bmuStatusFlagsExtended;
   unsigned int fanSpeed0;
   unsigned int fanSpeed1;
   unsigned int fanCurrentConsumption;
   unsigned int cmuCurrentConsumption;
   
   //Light fields
   bool leftBlinkerActivated;
   bool rightBlinkerActivated;
   bool headlightOn;
   bool brakelightOn;
   bool hazardsActivated;

   //OLED display fields
   bool bluetoothConnected;
   bool faultDetected;
   
   Serial pc;
};