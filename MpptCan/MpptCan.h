#pragma once
/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>

#include <MpptData.h>
class VehicleData;
union CanMessageFormatter;

class MpptCan
{
public:
   MpptCan(
      const PinName& canMpptTd,
      const PinName& canMpptRd,
      VehicleData& vehicleData);

   void initInterface();
   void sendCanData();

private:
   void readCan();

   void queryForDilithiumData();
   void sendMpptMode();
   void tryAndDecodeHelianthus(const CANMessage& receivedMessage);

   void writeFloatArrayToCharArray(const float* input, char* output);
   void writeCharArrayToFloat(const unsigned char* input, float* output);
   void populateCanMessageFormatter(const unsigned char* input, CanMessageFormatter& output);
   bool getHelianthusMessageNumbers(const unsigned int id, int& mpptNumber, int& messageNumber);

   void readDilithiumMessage(int mpptNumber, const unsigned char* messageData);
   void readStatus(int mpptNumber, const unsigned char* messageData);
   void readInput(int mpptNumber, const unsigned char* messageData);
   void readOutput(int mpptNumber, const unsigned char* messageData);

   void setupMpptData();
   int returnMpptDataIndexForMpptNumber(MpptData::Type type, int mpptNumber);

private:
   CAN mpptCan_;
   VehicleData& vehicleData_;
   int dilithiumQueryCounter_;
};
