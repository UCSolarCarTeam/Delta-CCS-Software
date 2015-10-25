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

#pragma once

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
