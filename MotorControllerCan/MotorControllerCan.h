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
 *  of the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     General Public License as published by 
 *  the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  The Schulich Delta Central Communication System is distributed 
 *  in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 *  without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     General 
 *  Public License along with the Schulich Delta Central Communication 
 *  System. If not, see <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#pragma once

#include <mbed.h>

class VehicleData;
union TritiumDataFormatter;

class MotorControllerCan
{
public:
   MotorControllerCan(
      const PinName& canTd,
      const PinName& canRd,
      const PinName& resetPin,
      VehicleData& vehicleData);

   void initInterface();
   void sendCanData();

private:
   void readCan();

   void sendSetBusCurrentALimitTo100Percent();
   void sendSetVelocityAndCurrent();

   void sendBmuIdleCommand();
   void sendBmuRunCommand();

   void requestResetOfMotorControllers();
   void sendResetMotorControllerOne();
   void sendResetMotorControllerTwo();
   void resetMotorControllers();

   void sendConfigurationMessage();

   void readStatus(const unsigned char* messageData, int motorNumber);
   void readBusCurrentA(const unsigned char* messageData);
   void readVelocity(const unsigned char* messageData);
   void readPhaseCurrent(const unsigned char* messageData);
   void readVoltageVector(const unsigned char* messageData);
   void readCurrentVector(const unsigned char* messageData);
   void readBackEmf(const unsigned char* messageData);
   void readTemperature(const unsigned char* messageData);
   void readDspTemperature(const unsigned char* messageData);

   void readCmuCellTemp(const unsigned char* messageData, int cmuCellNumber);
   void readCmuCellGroup1(const unsigned char* messageData, int cmuCellNumber);
   void readCmuCellGroup2(const unsigned char* messageData, int cmuCellNumber);
   void readStateOfCharge(const unsigned char* messageData);
   void readBalanceStateOfCharge(const unsigned char* messageData);
   void readPrechargeState(const unsigned char* messageData);
   void readPackVoltageCurrent(const unsigned char* messageData);
   void readPackStatus(const unsigned char* messageData);
   void readPackFanStatus(const unsigned char* messageData);
   void readExtendedPackStatus(const unsigned char* messageData);

   void writeFloatArrayToCharArray(const float* input, char* output);
   void writeCharArrayToFloat(const unsigned char* input, float* output);
   void populateTritiumDataFormatter(const unsigned char* input, TritiumDataFormatter& output);

   void checkIfOvercurrent();

private:
   CAN motorControllerCan_;
   InterruptIn resetInput_;
   volatile bool resetMotorControllers_;

   VehicleData& vehicleData_;
};
