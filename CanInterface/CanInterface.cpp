/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

// Solar car includes
#include <CanInterface.h>
#include <VehicleData.h>

//This structure the basic format of how the tritium sends its data.
//Unfortunatly it is not completely portable and you have to be very careful about the endianess
union TritiumDataFormatter 
{
   float floatData[2];
   unsigned char unsignedCharData[8];
   char charData[8];
   unsigned int unsignedIntData[4];
   int intData[4];
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

   const float METER_PER_SECOND_TO_KPH_CONVERSION = 3.6;
}

CanInterface::CanInterface(PinName canTd,
                           PinName canRd,
                           PinName canMpptTd,
                           PinName canMpptRd,
                           VehicleData& vehicleData)
: motorControllerCan_(canRd, canTd)
, mpptCan_(canMpptRd, canMpptTd)
, vehicleData_(vehicleData)
{
}

void CanInterface::initInterface()
{
   motorControllerCan_.frequency(CAN_FREQUENCY);
   motorControllerCan_.attach(this, &CanInterface::readCan);

   sendSetVelocityAndCurrent();
   wait_ms(20);

   sendConfigurationMessage();
   sendSetbusCurrentALimitTo100Percent();
   
   wait_ms(10);

   sendResetMotorControllerOne();
   sendResetMotorControllerTwo();
}

void CanInterface::readCan()
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
         readStatus(messageReceived.data);
         break;
      case MOTOR_ONE_BASE + BUS_MEASUREMENT:
         readbusCurrentA(messageReceived.data);
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

void CanInterface::sendCanData()
{
   sendSetVelocityAndCurrent();
}

//Needs to be sent at least once every 250 ms
//Format
//63-32 Motor current percent
//31-0 Velocity RPM
void CanInterface::sendSetVelocityAndCurrent()
{
   unsigned int canAddress = DRIVER_CONTROL_CAN_BASE + MOTOR_DRIVE;

   float dataToSendFloat[2] ;
   
   dataToSendFloat[0] = vehicleData_.driverSetSpeedRpm;
   dataToSendFloat[1] = vehicleData_.driverSetCurrent;

   char dataToSendChar[8];

   writeFloatArrayToCharArray(dataToSendFloat, dataToSendChar);

   CANMessage velocityAndCurrent(canAddress, dataToSendChar);
   motorControllerCan_.write(velocityAndCurrent);     
   
}

void CanInterface::sendSetbusCurrentALimitTo100Percent()
{
   unsigned int canAddress = DRIVER_CONTROL_CAN_BASE + POWER;
   float dataToSendFloat[2] ;
   
   dataToSendFloat[0] = 0.0f;
   dataToSendFloat[1] = 1.0f;

   char dataToSendChar[8];

   writeFloatArrayToCharArray(dataToSendFloat, dataToSendChar);

   CANMessage busLimit(canAddress, dataToSendChar);
   motorControllerCan_.write(busLimit);
}

void CanInterface::sendResetMotorControllerOne()
{
   unsigned int canAddress = MOTOR_ONE_BASE + RESET;

   TritiumDataFormatter messageDataFormatted;
   messageDataFormatted.unsignedLongData[1] = 0x00535754;
   messageDataFormatted.unsignedLongData[0] = 0x45534552;

   CANMessage motorReset(canAddress, messageDataFormatted.charData);
   motorControllerCan_.write(motorReset);
   
   canAddress = 0x503;
   CANMessage motorReset2(canAddress, messageDataFormatted.charData);
   motorControllerCan_.write(motorReset2);

}

void CanInterface::sendResetMotorControllerTwo()
{
   unsigned int canAddress = MOTOR_TWO_BASE + RESET;
   
   TritiumDataFormatter messageDataFormatted;
   messageDataFormatted.unsignedLongData[1] = 0x00535754;
   messageDataFormatted.unsignedLongData[0] = 0x45534552;

   CANMessage motorReset(canAddress, messageDataFormatted.charData);
   motorControllerCan_.write(motorReset);
}

void CanInterface::sendConfigurationMessage()
{
   unsigned int canAddress = DRIVER_CONTROL_CAN_BASE;
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

void CanInterface::readStatus(const unsigned char* messageData)
{
   vehicleData_.receivedErrorCount = messageData[7];
   vehicleData_.transmittedErrorCount = messageData[6];
   vehicleData_.activeMotor = messageData[5] << (8 + messageData[4]);
   vehicleData_.errorFlags = messageData[3] << (8 + messageData[2]);
   vehicleData_.limitFlags = messageData[1] << (8 + messageData[0]);
}

void CanInterface::readbusCurrentA(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.busCurrentA = receivedData[1];
   vehicleData_.busVoltage = receivedData[0];
}

void CanInterface::readVelocity(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.vehicleVelocityKph = receivedData[1] * METER_PER_SECOND_TO_KPH_CONVERSION;
   vehicleData_.motorVelocityRpm = receivedData[0];
}

void CanInterface::readPhaseCurrent(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.phaseCCurrent = receivedData[1];
   vehicleData_.phaseBCurrent = receivedData[0];
}

void CanInterface::readVoltageVector(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.motorVoltageReal = receivedData[1];
   vehicleData_.motorVoltageImaginary = receivedData[0];
}

void CanInterface::readCurrentVector(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.motorCurrentReal = receivedData[1];
   vehicleData_.motorCurrentImaginary = receivedData[0];
}

void CanInterface::readBackEmf(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   //Back EMF real is ignored because by definition it is always 0.
   vehicleData_.backEmfImaginary = receivedData[0];
}

void CanInterface::readTemperature(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.ipmHeatSinkTemp = receivedData[1];
   vehicleData_.motorTemp = receivedData[0];
}

void CanInterface::readDspTemperature(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   //Upper float not used.
   vehicleData_.dspBoardTemp = receivedData[0];
}

void CanInterface::readCmuCellTemp(const unsigned char* messageData, int cmuCellNumber)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.cmuData[cmuCellNumber].pcbTemperature = messageDataFormatted.intData[2] / 10.0;
   vehicleData_.cmuData[cmuCellNumber].cellTemperature = messageDataFormatted.intData[3] / 10.0;
}
void CanInterface::readCmuCellGroup1(const unsigned char* messageData, int cmuCellNumber)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[0] = messageDataFormatted.intData[0] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[1] = messageDataFormatted.intData[1] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[2] = messageDataFormatted.intData[2] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[3] = messageDataFormatted.intData[3] / 1000.0;
}
void CanInterface::readCmuCellGroup2(const unsigned char* messageData, int cmuCellNumber)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[4] = messageDataFormatted.intData[0] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[5] = messageDataFormatted.intData[1] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[6] = messageDataFormatted.intData[2] / 1000.0;
   vehicleData_.cmuData[cmuCellNumber].cellVoltage[7] = messageDataFormatted.intData[3] / 1000.0;
}
void CanInterface::readStateOfCharge(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.packStateOfCharge = receivedData[0];
   vehicleData_.packStateOfChargePercentage = receivedData[1];
}
void CanInterface::readBalanceStateOfCharge(const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);
   vehicleData_.balancePackStateOfCharge = receivedData[0];
   vehicleData_.balancePackStateOfChargePercentage = receivedData[1];
}
void CanInterface::readPrechargeState(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.prechargeDriverStatusFlags = messageDataFormatted.unsignedCharData[0];
   vehicleData_.prechargeState = messageDataFormatted.unsignedCharData[1];
   vehicleData_.contactorSupplyVoltage = messageDataFormatted.unsignedIntData[1];
   vehicleData_.prechargeTimerElapsedFlag = messageDataFormatted.unsignedCharData[6];
   vehicleData_.prechargeTimerCounter = messageDataFormatted.unsignedCharData[7];
}
void CanInterface::readPackVoltageCurrent(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.batteryVoltage = messageDataFormatted.unsignedLongData[0] / 1000.0;
   vehicleData_.batteryCurrent = messageDataFormatted.longData[1] / 1000.0;
}
void CanInterface::readPackStatus(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.batteryVoltageThresholdRising = messageDataFormatted.unsignedIntData[0];
   vehicleData_.batteryVoltageThresholdFalling = messageDataFormatted.unsignedIntData[1];
}
void CanInterface::readPackFanStatus(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.fanSpeed0 = messageDataFormatted.unsignedIntData[0];
   vehicleData_.fanSpeed1 = messageDataFormatted.unsignedIntData[1];
   vehicleData_.fanCurrentConsumption = messageDataFormatted.unsignedIntData[2];
   vehicleData_.cmuCurrentConsumption = messageDataFormatted.unsignedIntData[3];
}
void CanInterface::readExtendedPackStatus(const unsigned char* messageData)
{
   TritiumDataFormatter messageDataFormatted;
   populateTritiumDataFormatter(messageData, messageDataFormatted);
   vehicleData_.bmuStatusFlagsExtended = messageDataFormatted.unsignedLongData[0];
}

void CanInterface::writeFloatArrayToCharArray(const float* input, char* output)
{
   memcpy(output, input, sizeof(float)*2);
}

void CanInterface::writeCharArrayToFloat(const unsigned char* input, float* output)
{
   memcpy(output, input, sizeof(float)*2);
}

void CanInterface::populateTritiumDataFormatter(const unsigned char* input, TritiumDataFormatter& output)
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
