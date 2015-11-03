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

#include <MpptCan.h>
#include <VehicleData.h>

union CanMessageFormatter
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
   const unsigned int CAN_FREQUENCY = 125000;
   const int DILITHIUM_QUERY_COUNTER = 20;

   enum Addresses
   {
      Dilithium1 = 0x0601,
      Dilithium2 = 0x0602,
      Dilithium3 = 0x0603,
      Dilithium1State = 0x0611,
      Dilithium2State = 0x0612,
      Dilithium3State = 0x0613,
      HelianthusBase = 0x400,
      HelianthusModeSelection = 0x300
   };
}

MpptCan::MpptCan(
   const PinName& canMpptTd,
   const PinName& canMpptRd,
   VehicleData& vehicleData)
: mpptCan_(canMpptRd, canMpptTd)
, vehicleData_(vehicleData)
, dilithiumQueryCounter_(0)
{
}

void MpptCan::initInterface()
{
   mpptCan_.frequency(CAN_FREQUENCY);
   mpptCan_.attach(this, &MpptCan::readCan);
   setupMpptData();
   wait_ms(10);
}

void MpptCan::readCan()
{
   CANMessage messageReceived;
   mpptCan_.read(messageReceived);

   if (messageReceived.type == CANRemote)
   {
      //If we ever need to implement something to respond to queries
   }
   else
   {
      switch(messageReceived.id)
      {
      case Dilithium1:
         readDilithiumMessage(1, messageReceived.data);
         break;
      case Dilithium2:
         readDilithiumMessage(2, messageReceived.data);
         break;
      case Dilithium3:
         readDilithiumMessage(3, messageReceived.data);
         break;
      default:
         tryAndDecodeHelianthus(messageReceived);
         break;
      }
   }
}

void MpptCan::sendCanData()
{
   if (dilithiumQueryCounter_ == 0)
   {
      queryForDilithiumData();
      sendMpptMode();
      dilithiumQueryCounter_ = DILITHIUM_QUERY_COUNTER;
   }
   else if(dilithiumQueryCounter_ == 1)
   {
      setDilithiumState(vehicleData_.arrayActivated);
   }

   dilithiumQueryCounter_--;
}

void MpptCan::queryForDilithiumData()
{
   CANMessage message1(Dilithium1);
   CANMessage message2(Dilithium2);
   CANMessage message3(Dilithium3);
   mpptCan_.write(message1);
   mpptCan_.write(message2);
   mpptCan_.write(message3);
}

void MpptCan::setDilithiumState(bool mpptsOn)
{
   CANMessage message1;
   CANMessage message2;
   CANMessage message3;
   message1.id = Dilithium1State;
   message2.id = Dilithium2State;
   message3.id = Dilithium3State;
   message1.data[0] = static_cast<char>(mpptsOn);
   message2.data[0] = static_cast<char>(mpptsOn);
   message3.data[0] = static_cast<char>(mpptsOn);
   mpptCan_.write(message1);
   mpptCan_.write(message2);
   mpptCan_.write(message3);
}

void MpptCan::sendMpptMode()
{
   CANMessage message;
   message.id = static_cast<int>(HelianthusModeSelection);

   if (vehicleData_.arrayActivated)
   {
      message.data[0] = vehicleData_.dynamicModeActivated ? MpptData::Dynamic : MpptData::Static;
   }
   else
   {
      message.data[0] = MpptData::Open;
   }

   mpptCan_.write(message);
}

void MpptCan::tryAndDecodeHelianthus(const CANMessage& receivedMessage)
{
   int mpptNumber = 0;
   int messageNumber = 0;

   if (!getHelianthusMessageNumbers(receivedMessage.id, mpptNumber, messageNumber))
   {
      return;
   }

   switch (messageNumber)
   {
   case 0:
      readStatus(mpptNumber, receivedMessage.data);
      break;
   case 1:
      readInput(mpptNumber, receivedMessage.data);
      break;
   case 2:
      readOutput(mpptNumber, receivedMessage.data);
      break;
   default:
      break;
   }
}

void MpptCan::readDilithiumMessage(int mpptNumber, const unsigned char* messageData)
{
   CanMessageFormatter formatter;
   populateCanMessageFormatter(messageData, formatter);
   const int mpptDataIndex = returnMpptDataIndexForMpptNumber(MpptData::Dilithium, mpptNumber);
   vehicleData_.mpptData[mpptDataIndex].voltageIn = formatter.unsignedShortData[0] / 100.0;
   vehicleData_.mpptData[mpptDataIndex].currentIn = formatter.unsignedShortData[1] / 1000.0;
   vehicleData_.mpptData[mpptDataIndex].voltageOut = formatter.unsignedShortData[2] / 100.0;
   vehicleData_.mpptData[mpptDataIndex].currentOut = -1.0f;
}

void MpptCan::readStatus(int mpptNumber, const unsigned char* messageData)
{
   unsigned char mode = messageData[0];
   // unsigned char warnings = messageData[1];

   const int mpptDataIndex = returnMpptDataIndexForMpptNumber(MpptData::Helianthus, mpptNumber);
   vehicleData_.mpptData[mpptDataIndex].mode = static_cast<MpptData::Mode>(mode);
}

void MpptCan::readInput(int mpptNumber, const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);

   const int mpptDataIndex = returnMpptDataIndexForMpptNumber(MpptData::Helianthus, mpptNumber);
   vehicleData_.mpptData[mpptDataIndex].voltageIn = receivedData[0];
   vehicleData_.mpptData[mpptDataIndex].currentIn = receivedData[1];
}

void MpptCan::readOutput(int mpptNumber, const unsigned char* messageData)
{
   float receivedData[2];
   writeCharArrayToFloat(messageData, receivedData);

   const int mpptDataIndex = returnMpptDataIndexForMpptNumber(MpptData::Helianthus, mpptNumber);
   vehicleData_.mpptData[mpptDataIndex].voltageOut = receivedData[0];
   vehicleData_.mpptData[mpptDataIndex].currentOut = receivedData[1];
}

bool MpptCan::getHelianthusMessageNumbers(const unsigned int id, int& mpptNumber, int& messageNumber)
{
   if ((id & 0xF00) == HelianthusBase)
   {
      mpptNumber = (id & 0x0F0) >> 8;
      messageNumber = (id & 0x00F);

      if (mpptNumber <= 4 && messageNumber <= 3)
      {
         return true;
      }
   }
   return false;
}

void MpptCan::writeFloatArrayToCharArray(const float* input, char* output)
{
   memcpy(output, input, sizeof(float) * 2);
}

void MpptCan::writeCharArrayToFloat(const unsigned char* input, float* output)
{
   memcpy(output, input, sizeof(float)*2);
}

void MpptCan::populateCanMessageFormatter(const unsigned char* input, CanMessageFormatter& output)
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

void MpptCan::setupMpptData()
{
   vehicleData_.mpptData[0].type = MpptData::Dilithium;
   vehicleData_.mpptData[1].type = MpptData::Dilithium;
   vehicleData_.mpptData[2].type = MpptData::Dilithium;
   vehicleData_.mpptData[0].mode = MpptData::Invalid;
   vehicleData_.mpptData[1].mode = MpptData::Invalid;
   vehicleData_.mpptData[2].mode = MpptData::Invalid;

   vehicleData_.mpptData[3].type = MpptData::Helianthus;
   vehicleData_.mpptData[4].type = MpptData::Helianthus;
   vehicleData_.mpptData[5].type = MpptData::Helianthus;
   vehicleData_.mpptData[6].type = MpptData::Helianthus;
}

int MpptCan::returnMpptDataIndexForMpptNumber(MpptData::Type type, int mpptNumber)
{
   if (type == MpptData::Dilithium)
   {
      switch(mpptNumber)
      {
      default:
      case 1:
         return 0;
      case 2:
         return 1;
      case 3:
         return 2;
      }
   }
   else
   {
      switch(mpptNumber)
      {
      default:
      case 1:
         return 3;
      case 2:
         return 4;
      case 3:
         return 5;
      case 4:
         return 6;
      }
   }
}
