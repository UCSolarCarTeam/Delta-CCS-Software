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

#include <CcsDefines.h>
#include <MotorControllerCan.h>
#include <VehicleData.h>

//This structure the basic format of how the tritium sends its data.
//Unfortunatly it is not completely portable and you have to be very careful about the endianess
union TritiumDataFormatter
{
   float floatData[2];
   unsigned char unsignedCharData[8];
   char charData[8];
   unsigned short unsignedShortData[4];
   short shortData[4];
   unsigned long unsignedLongData[2];
   long longData[2];
};

namespace
{
   const unsigned int CAN_FREQUENCY = 500000;

//BMU information
   enum BmuAddresses
   {
      base = 0x0600,
      cmu0Temp = 0x0601,
      cmu0CellGroup1 = 0x0602,
      cmu0CellGroup2 = 0x0603,
      cmu1Temp = 0x0604,
      cmu1CellGroup1 = 0x0605,
      cmu1CellGroup2 = 0x0606,
      cmu2Temp = 0x0607,
      cmu2CellGroup1 = 0x0608,
      cmu2CellGroup2 = 0x0609,
      cmu3Temp = 0x060A,
      cmu3CellGroup1 = 0x060B,
      cmu3CellGroup2 = 0x060C,
      stateOfCharge = 0x06F4,
      balanceStateOfCharge = 0x06F5,
      prechargeState = 0x06F7,
      packVoltageCurrent = 0x06FA,
      packStatus = 0x06FB,
      packFanStatus = 0x06FC,
      extendedPackStatus = 0x06FD,
   };

// Driver controls CAN base address and packet offsets
   const unsigned int DRIVER_CONTROL_CAN_BASE = 0x500;
   const unsigned int MOTOR_DRIVE = 1;
   const unsigned int POWER = 2;
   const unsigned int BMU_STATE = 5;

// Motor control CAN base address and offsets
   const unsigned int MOTOR_ONE_BASE = 0x0400;
   const unsigned int MOTOR_TWO_BASE = 0x0420;
   const unsigned int STATUS = 1;
   const unsigned int BUS_MEASUREMENT = 2;
   const unsigned int VELOCITY_MEASUREMENT = 3;
   const unsigned int PHASE_CURRENT_MEASUREMENT = 4;
   const unsigned int VOLTAGE_VECTOR_MEASUREMENT = 5;
   const unsigned int CURRENT_VECTOR_MEASUREMENT = 6;
   const unsigned int BACK_EMF_MEASUREMENT = 7;
   const unsigned int TEMPERATURE_MEASUREMENT = 11;
   const unsigned int DSP_TEMPERATURE_MEASUREMENT = 12;
   const unsigned int ODOMETER_MEASUREMENT = 14;

   const unsigned int RESET = 25;

   const unsigned int DEVICE_SERIAL_NUMBER = 0x01;

   // BMU state defines
   // These numbers are not on any data sheet. Taken from
   // APU code 2015-07-12
   const unsigned char BMU_RUN_COMMAND = 0x72;
   const unsigned char BMU_IDLE_COMMAND = 0x02;
}

MotorControllerCan::MotorControllerCan(
   const PinName& canTd,
   const PinName& canRd,
   const PinName& resetPin,
   VehicleData& vehicleData)
: motorControllerCan_(canRd, canTd)
, resetInput_(resetPin)
, resetMotorControllers_(false)
, vehicleData_(vehicleData)
{
   resetInput_.rise(this, &MotorControllerCan::resetMotorControllers);
}

void MotorControllerCan::initInterface()
{
   motorControllerCan_.frequency(CAN_FREQUENCY);
   motorControllerCan_.attach(this, &MotorControllerCan::readCan);

   sendSetVelocityAndCurrent();
   wait_ms(20);

   sendConfigurationMessage();
   sendSetBusCurrentALimitTo100Percent();

   wait_ms(10);
}

void MotorControllerCan::readCan()
{
   CANMessage messageReceived;
   motorControllerCan_.read(messageReceived);

   if (messageReceived.type == CANRemote)
   {
      //If we ever need to implement something to respond to queries
   }
   else
   {
      switch(messageReceived.id)
      {
      case MOTOR_ONE_BASE + STATUS:
         readStatus(messageReceived.data, 1);
         break;
      case MOTOR_TWO_BASE + STATUS:
         readStatus(messageReceived.data, 2);
         break;
      case MOTOR_ONE_BASE + BUS_MEASUREMENT:
         readBusCurrentA(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + VELOCITY_MEASUREMENT:
         readVelocity(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + PHASE_CURRENT_MEASUREMENT:
         readPhaseCurrent(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + VOLTAGE_VECTOR_MEASUREMENT:
         readVoltageVector(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + CURRENT_VECTOR_MEASUREMENT:
         readCurrentVector(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + BACK_EMF_MEASUREMENT:
         readBackEmf(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + TEMPERATURE_MEASUREMENT:
         readTemperature(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + DSP_TEMPERATURE_MEASUREMENT:
         readDspTemperature(messageReceived.data);
         break;
      case cmu0Temp:
         readCmuCellTemp(messageReceived.data, 0);
         break;
      case cmu0CellGroup1:
         readCmuCellGroup1(messageReceived.data, 0);
         break;
      case cmu0CellGroup2:
         readCmuCellGroup2(messageReceived.data, 0);
         break;
      case cmu1Temp:
         readCmuCellTemp(messageReceived.data, 1);
         break;
      case cmu1CellGroup1:
         readCmuCellGroup1(messageReceived.data, 1);
         break;
      case cmu1CellGroup2:
         readCmuCellGroup2(messageReceived.data, 1);
         break;
      case cmu2Temp:
         readCmuCellTemp(messageReceived.data, 2);
         break;
      case cmu2CellGroup1:
         readCmuCellGroup1(messageReceived.data, 2);
         break;
      case cmu2CellGroup2:
         readCmuCellGroup2(messageReceived.data, 2);
         break;
      case cmu3Temp:
         readCmuCellTemp(messageReceived.data, 3);
         break;
      case cmu3CellGroup1:
         readCmuCellGroup1(messageReceived.data, 3);
         break;
      case cmu3CellGroup2:
         readCmuCellGroup2(messageReceived.data, 3);
         break;
      case stateOfCharge:
         readStateOfCharge(messageReceived.data);
         break;
      case balanceStateOfCharge:
         readBalanceStateOfCharge(messageReceived.data);
         break;
      case prechargeState:
         readPrechargeState(messageReceived.data);
         break;
      case packVoltageCurrent:
         readPackVoltageCurrent(messageReceived.data);
         break;
      case packStatus:
         readPackStatus(messageReceived.data);
         break;
      case packFanStatus:
         readPackFanStatus(messageReceived.data);
         break;
      case extendedPackStatus:
         readExtendedPackStatus(messageReceived.data);
         break;
      }
   }
}

void MotorControllerCan::sendCanData()
{
   if (resetMotorControllers_)
   {
      resetMotorControllers();
   }

   checkIfOvercurrent();

   if (vehicleData_.highVoltageActivated &&
      !vehicleData_.overcurrentProtectionTripped)
   {
      sendBmuRunCommand();
   }
   else
   {
      sendBmuIdleCommand();
   }

   if (!vehicleData_.highVoltageActivated)
   {
      vehicleData_.overcurrentProtectionTripped = false;
   }

   sendSetVelocityAndCurrent();
}

//Needs to be sent at least once every 250 ms
//Format
//63-32 Motor current percent
//31-0 Velocity RPM
void MotorControllerCan::sendSetVelocityAndCurrent()
{
   const unsigned int canAddress = DRIVER_CONTROL_CAN_BASE + MOTOR_DRIVE;

   float dataToSendFloat[2];

   dataToSendFloat[0] = vehicleData_.driverSetSpeedRpm;
   dataToSendFloat[1] = vehicleData_.driverSetCurrentPercentage;

   char dataToSendChar[8];

   writeFloatArrayToCharArray(dataToSendFloat, dataToSendChar);

   CANMessage velocityAndCurrent(canAddress, dataToSendChar);
   motorControllerCan_.write(velocityAndCurrent);
}

void MotorControllerCan::sendBmuIdleCommand()
{
   CANMessage message;
   message.id = DRIVER_CONTROL_CAN_BASE + BMU_STATE;
   message.data[0] = BMU_IDLE_COMMAND;
   motorControllerCan_.write(message);
}

void MotorControllerCan::sendBmuRunCommand()
{
   CANMessage message;
   message.id = DRIVER_CONTROL_CAN_BASE + BMU_STATE;
   message.data[0] = BMU_RUN_COMMAND;
   motorControllerCan_.write(message);
}

void MotorControllerCan::sendSetBusCurrentALimitTo100Percent()
{
   const unsigned int canAddress = DRIVER_CONTROL_CAN_BASE + POWER;
   float dataToSendFloat[2];

   dataToSendFloat[0] = 0.0f;
   dataToSendFloat[1] = 1.0f;

   char dataToSendChar[8];

   writeFloatArrayToCharArray(dataToSendFloat, dataToSendChar);

   CANMessage busLimit(canAddress, dataToSendChar);
   motorControllerCan_.write(busLimit);
}

void MotorControllerCan::requestResetOfMotorControllers()
{
   resetMotorControllers_ = true;
}

void MotorControllerCan::sendResetMotorControllerOne()
{
   const unsigned int canAddress = MOTOR_ONE_BASE + RESET;

   TritiumDataFormatter messageDataFormatted;
   messageDataFormatted.unsignedLongData[1] = 0x00535754;
   messageDataFormatted.unsignedLongData[0] = 0x45534552;

   CANMessage motorReset(canAddress, messageDataFormatted.charData);
   motorControllerCan_.write(motorReset);

   const unsigned int legacyResetCanAddress = 0x503;
   CANMessage motorResetLegacy(legacyResetCanAddress, messageDataFormatted.charData);
   motorControllerCan_.write(motorResetLegacy);
}

void MotorControllerCan::sendResetMotorControllerTwo()
{
   const unsigned int canAddress = MOTOR_TWO_BASE + RESET;

   TritiumDataFormatter messageDataFormatted;
   messageDataFormatted.unsignedLongData[1] = 0x00535754;
   messageDataFormatted.unsignedLongData[0] = 0x45534552;

   CANMessage motorReset(canAddress, messageDataFormatted.charData);
   motorControllerCan_.write(motorReset);
}

void MotorControllerCan::resetMotorControllers()
{
   sendResetMotorControllerOne();
   sendResetMotorControllerTwo();
   resetMotorControllers_ = false;
}

void MotorControllerCan::sendConfigurationMessage()
{
   const unsigned int canAddress = DRIVER_CONTROL_CAN_BASE;
   char data[8];
   data[7] = 'T';
   data[6] = 'R';
   data[5] = 'I';
   data[4] = 'b';
   data[3] = 0x00;
   data[2] = 0x00;
   data[1] = 0x00;
   data[0] = DEVICE_SERIAL_NUMBER;

   CANMessage configurationMessage(canAddress, data);
   motorControllerCan_.write(configurationMessage);
}

void MotorControllerCan::readStatus(const unsigned char* messageData, int motorNumber)
{
   if (motorNumber == 1)
   {
      vehicleData_.motorOneReceivedErrorCount = messageData[7];
      vehicleData_.motorOneTransmittedErrorCount = messageData[6];
      vehicleData_.motorOneActiveMotor = (messageData[5] << 8) | messageData[4];
      vehicleData_.motorOneErrorFlags = (messageData[3] << 8) | messageData[2];
      vehicleData_.motorOneLimitFlags = (messageData[1] << 8) | messageData[0];
   }
   else
   {
      vehicleData_.motorTwoReceivedErrorCount = messageData[7];
      vehicleData_.motorTwoTransmittedErrorCount = messageData[6];
      vehicleData_.motorTwoActiveMotor = (messageData[5] << 8) | messageData[4];
      vehicleData_.motorTwoErrorFlags = (messageData[3] << 8) | messageData[2];
      vehicleData_.motorTwoLimitFlags = (messageData[1] << 8) | messageData[0];
   }
}

void MotorControllerCan::readBusCurrentA(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.busCurrent = receivedData[1];
   vehicleData_.busVoltage = receivedData[0];
}

void MotorControllerCan::readVelocity(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.vehicleVelocity = receivedData[1];
   vehicleData_.motorVelocityRpm = receivedData[0];
}

void MotorControllerCan::readPhaseCurrent(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.phaseCCurrent = receivedData[1];
   vehicleData_.phaseBCurrent = receivedData[0];
}

void MotorControllerCan::readVoltageVector(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.motorVoltageReal = receivedData[1];
   vehicleData_.motorVoltageImaginary = receivedData[0];
}

void MotorControllerCan::readCurrentVector(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.motorCurrentReal = receivedData[1];
   vehicleData_.motorCurrentImaginary = receivedData[0];
}

void MotorControllerCan::readBackEmf(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   //Back EMF real is ignored because by definition it is always 0.
   vehicleData_.backEmfImaginary = receivedData[0];
}

void MotorControllerCan::readTemperature(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.ipmHeatSinkTemp = receivedData[1];
   vehicleData_.motorTemp = receivedData[0];
}

void MotorControllerCan::readDspTemperature(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   //Upper float not used.
   vehicleData_.dspBoardTemp = receivedData[0];
}

void MotorControllerCan::readCmuCellTemp(const unsigned char* messageData, int cmuCellNumber)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.cmuData[cmuCellNumber].pcbTemperature = messageDataFormatted.shortData[2] / 10.0;
   vehicleData_.cmuData[cmuCellNumber].cellTemperature = messageDataFormatted.shortData[3] / 10.0;
}
void MotorControllerCan::readCmuCellGroup1(const unsigned char* messageData, int cmuCellNumber)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[0] = messageDataFormatted.shortData[0] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[1] = messageDataFormatted.shortData[1] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[2] = messageDataFormatted.shortData[2] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[3] = messageDataFormatted.shortData[3] / 1000.0;
}
void MotorControllerCan::readCmuCellGroup2(const unsigned char* messageData, int cmuCellNumber)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[4] = messageDataFormatted.shortData[0] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[5] = messageDataFormatted.shortData[1] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[6] = messageDataFormatted.shortData[2] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[7] = messageDataFormatted.shortData[3] / 1000.0;
}
void MotorControllerCan::readStateOfCharge(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.packStateOfCharge = receivedData[0];
   vehicleData_.packStateOfChargePercentage = receivedData[1];
}
void MotorControllerCan::readBalanceStateOfCharge(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.balancePackStateOfCharge = receivedData[0];
   vehicleData_.balancePackStateOfChargePercentage = receivedData[1];
}
void MotorControllerCan::readPrechargeState(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.prechargeDriverStatusFlags = messageDataFormatted.unsignedCharData[0];
   vehicleData_.prechargeState =
      static_cast<VehicleData::PrechargeStates>(messageDataFormatted.unsignedCharData[1]);
   vehicleData_.contactorSupplyVoltage = messageDataFormatted.unsignedShortData[1];
   vehicleData_.prechargeTimerElapsedFlag = messageDataFormatted.unsignedCharData[6];
   vehicleData_.prechargeTimerCounter = messageDataFormatted.unsignedCharData[7];
}
void MotorControllerCan::readPackVoltageCurrent(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.batteryVoltage = messageDataFormatted.unsignedLongData[0] / 1000.0;
   vehicleData_.batteryCurrent = messageDataFormatted.longData[1] / 1000.0;
}
void MotorControllerCan::readPackStatus(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.batteryVoltageThresholdRising = messageDataFormatted.unsignedShortData[0];
   vehicleData_.batteryVoltageThresholdFalling = messageDataFormatted.unsignedShortData[1];
}
void MotorControllerCan::readPackFanStatus(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.fanSpeed0 = messageDataFormatted.unsignedShortData[0];
   vehicleData_.fanSpeed1 = messageDataFormatted.unsignedShortData[1];
   vehicleData_.fanCurrentConsumption = messageDataFormatted.unsignedShortData[2];
   vehicleData_.cmuCurrentConsumption = messageDataFormatted.unsignedShortData[3];
}
void MotorControllerCan::readExtendedPackStatus(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.bmuStatusFlagsExtended = messageDataFormatted.unsignedLongData[0];
}

void MotorControllerCan::writeFloatArrayToCharArray(const float* input, char* output)
{
   memcpy(output, input, sizeof(float)*2);
}

void MotorControllerCan::writeCharArrayToFloat(const unsigned char* input, float* output)
{
   memcpy(output, input, sizeof(float)*2);
}

void MotorControllerCan::populateTritiumDataFormatter(const unsigned char* input, TritiumDataFormatter& output)
{
   output.unsignedCharData[0] = input[0];
   output.unsignedCharData[1] = input[1];
   output.unsignedCharData[2] = input[2];
   output.unsignedCharData[3] = input[3];
   output.unsignedCharData[4] = input[4];
   output.unsignedCharData[5] = input[5];
   output.unsignedCharData[6] = input[6];
   output.unsignedCharData[7] = input[7];
}

void MotorControllerCan::checkIfOvercurrent()
{
   if ((vehicleData_.batteryCurrent < CcsDefines::MAX_BATTERY_CURRENT_DRAIN) ||
      (vehicleData_.batteryCurrent > CcsDefines::MAX_BATTERY_CURRENT_CHARGE))
   {
      vehicleData_.overcurrentProtectionTripped = true;
   }
}
