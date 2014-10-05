/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

#pragma once

// Mbed includes
#include <mbed.h>

//Solar car includes
class VehicleData;
union TritiumDataFormatter;

class CanInterface
{
public:
   CanInterface(PinName canTd,
                PinName canRd,
                PinName canMpptTd,
                PinName canMpptRd,
                VehicleData& vehicleData);

   void initInterface();
   void sendCanData();

private:
   void readCan();
   
   void sendSetbusCurrentALimitTo100Percent();
   void sendSetVelocityAndCurrent();
   void sendResetMotorControllerOne();
   void sendResetMotorControllerTwo();
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
   
   VehicleData& vehicleData_;
};
