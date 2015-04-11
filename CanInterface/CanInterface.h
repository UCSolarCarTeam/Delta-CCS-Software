#pragma once
/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>

class VehicleData;
union TritiumDataFormatter;

class CanInterface
{
public:
   CanInterface(const PinName& canTd,
                const PinName& canRd,
                const PinName& canMpptTd,
                const PinName& canMpptRd,
                const PinName& resetPin,
                VehicleData& vehicleData);

   void initInterface();
   void sendCanData();

private:
   void readCan();

   void sendSetbusCurrentALimitTo100Percent();
   void sendSetVelocityAndCurrent();

   void requestResetOfMotorControllers();
   void sendResetMotorControllerOne();
   void sendResetMotorControllerTwo();
   void resetMotorControllers();

   void sendConfigurationMessage();

   void readStatus(const unsigned char* messageData);
   void readbusCurrentA(const unsigned char* messageData);
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

private:
   CAN motorControllerCan_;
   CAN mpptCan_;
   InterruptIn resetInput_;
   volatile bool resetMotorControllers_;

   VehicleData& vehicleData_;
};
