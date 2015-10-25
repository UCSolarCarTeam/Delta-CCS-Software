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

#define BLUETOOTH 0

#include <CcsDefines.h>
#include <CrcCalculator.h>
#include <TelemetryReporting.h>
#include <VehicleData.h>

union FloatDataUnion
{
   float floatData;
   char charData[4];
};

namespace
{
   unsigned int BAUD_RATE = 9600;
   unsigned int NUMBER_OF_BITS = 8;
   unsigned int STOP_BIT = 1;
   SerialBase::Parity PARITY = SerialBase::None;

   // These lengths only include the data. Not the checksum
   const unsigned int KEY_DRIVER_CONTROL_LENGTH = 21;
   const unsigned int DRIVER_CONTROL_DETAILS_LENGTH = 29;
   const unsigned int FAULT_LENGTH = 9;
   const unsigned int BATTERY_DATA_LENGTH = 18;
   const unsigned int CMU_DATA_LENGTH = 42;
   const unsigned int MPPT_DATA_LENGTH = 21;
   const unsigned int CHECKSUM_LENGTH = 2;

   const unsigned int FRAMING_LENGTH_INCREASE = 5;
   const unsigned char TERMINATING_BYTE = 0x00;

   // Packet IDs
   const unsigned char KEY_DRIVER_CONTROL_ID = 0x01;
   const unsigned char DRIVER_CONTROL_DETAILS_ID = 0x02;
   const unsigned char FAULT_ID = 0x03;
   const unsigned char BATTERY_DATA_ID = 0x04;
   const unsigned char CMU_DATA_ID = 0x05;
   const unsigned char MPPT_DATA_ID = 0x06;
}

TelemetryReporting::TelemetryReporting(PinName uartTx,
                                       PinName uartRx,
                                       VehicleData& vehicleData)
: uart_(uartTx, uartRx)
, vehicleData_(vehicleData)
, timer_(0)
{
}

void TelemetryReporting::initTelemetryReporting()
{
   uart_.baud(BAUD_RATE);
   uart_.format(NUMBER_OF_BITS, PARITY, STOP_BIT);

#if BLUETOOTH
   uart_.puts("AT\n");
   uart_.puts("SET BT AUTH * 0001\n");
   uart_.puts("SET BT PAGEMODE 3 3000 1\n");
#endif
}

void TelemetryReporting::transmitTelemetry()
{
   sendKeyDriverControlTelemetry();
   sendFaults();

   switch (timer_)
   {
   case 0:
      sendDriverControlDetails();
      sendMpptData(0);
      break;
   case 1:
      sendBatteryData();
      sendMpptData(1);
      break;
   case 2:
      sendCmuData(0);
      sendMpptData(2);
      break;
   case 3:
      sendCmuData(1);
      sendMpptData(3);
      break;
   case 4:
      sendCmuData(2);
      sendMpptData(4);
      break;
   case 5:
      sendCmuData(3);
      sendMpptData(5);
      break;
   case 6:
      sendMpptData(6);
      break;
   }

   if (timer_ >= 6)
   {
      timer_ = 0;
   }
   else
   {
      timer_++;
   }
}

void TelemetryReporting::sendKeyDriverControlTelemetry()
{
   const unsigned int unframedPacketLength = KEY_DRIVER_CONTROL_LENGTH + CHECKSUM_LENGTH;
   unsigned char packetPayload[unframedPacketLength];
   packetPayload[0] = KEY_DRIVER_CONTROL_ID;
   const float driverSetSpeedMps = vehicleData_.driverSetSpeedRpm * CcsDefines::RPM_TO_MPS_CONVERSION;
   writeFloatIntoData(packetPayload, 1, driverSetSpeedMps);
   writeFloatIntoData(packetPayload, 5, vehicleData_.driverSetCurrent);
   writeFloatIntoData(packetPayload, 9, vehicleData_.busCurrent);
   writeFloatIntoData(packetPayload, 13, vehicleData_.busVoltage);
   writeFloatIntoData(packetPayload, 17, vehicleData_.vehicleVelocity);

   addChecksum(packetPayload, KEY_DRIVER_CONTROL_LENGTH);
   unsigned char packet[unframedPacketLength + FRAMING_LENGTH_INCREASE];
   unsigned int packetLength = frameData(packetPayload, unframedPacketLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendDriverControlDetails()
{
   const unsigned int unframedPacketLength = DRIVER_CONTROL_DETAILS_LENGTH + CHECKSUM_LENGTH;
   unsigned char packetPayload[unframedPacketLength];
   packetPayload[0] = DRIVER_CONTROL_DETAILS_ID;
   writeFloatIntoData(packetPayload, 1, vehicleData_.motorVelocityRpm);
   writeFloatIntoData(packetPayload, 5, vehicleData_.motorVoltageReal);
   writeFloatIntoData(packetPayload, 9, vehicleData_.motorCurrentReal);
   writeFloatIntoData(packetPayload, 13, vehicleData_.backEmfImaginary);
   writeFloatIntoData(packetPayload, 17, vehicleData_.dspBoardTemp);
   writeFloatIntoData(packetPayload, 21, vehicleData_.dcBusAmpHours);
   writeFloatIntoData(packetPayload, 25, vehicleData_.odometer);

   addChecksum(packetPayload, DRIVER_CONTROL_DETAILS_LENGTH);
   unsigned char packet[unframedPacketLength + FRAMING_LENGTH_INCREASE];
   unsigned int packetLength = frameData(packetPayload, unframedPacketLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendFaults()
{
   const unsigned int unframedPacketLength = FAULT_LENGTH + CHECKSUM_LENGTH;
   unsigned char packetPayload[unframedPacketLength];
   packetPayload[0] = FAULT_ID;
   packetPayload[1] = static_cast<unsigned char>(0xFF & vehicleData_.motorOneErrorFlags);
   packetPayload[2] = static_cast<unsigned char>(0xFF & vehicleData_.motorOneLimitFlags);
   packetPayload[3] = static_cast<unsigned char>(0xFF & vehicleData_.motorTwoErrorFlags);
   packetPayload[4] = static_cast<unsigned char>(0xFF & vehicleData_.motorTwoLimitFlags);
   packetPayload[5] = static_cast<unsigned char>(0xFF & vehicleData_.bmuStatusFlagsExtended);
   packetPayload[6] = static_cast<unsigned char>(0xFF & (vehicleData_.bmuStatusFlagsExtended >> 8));
   packetPayload[7] = vehicleData_.motorOneReceivedErrorCount;
   packetPayload[8] = vehicleData_.motorOneTransmittedErrorCount;

   addChecksum(packetPayload, FAULT_LENGTH);
   unsigned char packet[unframedPacketLength + FRAMING_LENGTH_INCREASE];
   unsigned int packetLength = frameData(packetPayload, unframedPacketLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendBatteryData()
{
   const unsigned int unframedPacketLength = BATTERY_DATA_LENGTH + CHECKSUM_LENGTH;
   unsigned char packetPayload[unframedPacketLength];
   packetPayload[0] = BATTERY_DATA_ID;
   writeFloatIntoData(packetPayload, 1, vehicleData_.batteryVoltage);
   writeFloatIntoData(packetPayload, 5, vehicleData_.batteryCurrent);
   writeFloatIntoData(packetPayload, 9, vehicleData_.packStateOfChargePercentage);
   writeFloatIntoData(packetPayload, 13, vehicleData_.balancePackStateOfCharge);
   packetPayload[17] = static_cast<unsigned char>(vehicleData_.secondaryBatteryUnderVoltage);

   addChecksum(packetPayload, BATTERY_DATA_LENGTH);
   unsigned char packet[unframedPacketLength + FRAMING_LENGTH_INCREASE];
   unsigned int packetLength = frameData(packetPayload, unframedPacketLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendCmuData(unsigned char cmuDataIndex)
{
   const unsigned int unframedPacketLength = CMU_DATA_LENGTH + CHECKSUM_LENGTH;
   unsigned char packetPayload[unframedPacketLength];
   packetPayload[0] = CMU_DATA_ID;
   packetPayload[1] = cmuDataIndex;
   writeFloatIntoData(packetPayload, 2, vehicleData_.cmuData[cmuDataIndex].pcbTemperature);
   writeFloatIntoData(packetPayload, 6, vehicleData_.cmuData[cmuDataIndex].cellTemperature);
   writeFloatIntoData(packetPayload, 10, vehicleData_.cmuData[cmuDataIndex].cellVoltage[0]);
   writeFloatIntoData(packetPayload, 14, vehicleData_.cmuData[cmuDataIndex].cellVoltage[1]);
   writeFloatIntoData(packetPayload, 18, vehicleData_.cmuData[cmuDataIndex].cellVoltage[2]);
   writeFloatIntoData(packetPayload, 22, vehicleData_.cmuData[cmuDataIndex].cellVoltage[3]);
   writeFloatIntoData(packetPayload, 26, vehicleData_.cmuData[cmuDataIndex].cellVoltage[4]);
   writeFloatIntoData(packetPayload, 30, vehicleData_.cmuData[cmuDataIndex].cellVoltage[5]);
   writeFloatIntoData(packetPayload, 34, vehicleData_.cmuData[cmuDataIndex].cellVoltage[6]);
   writeFloatIntoData(packetPayload, 38, vehicleData_.cmuData[cmuDataIndex].cellVoltage[7]);

   addChecksum(packetPayload, CMU_DATA_LENGTH);
   unsigned char packet[unframedPacketLength + FRAMING_LENGTH_INCREASE];
   unsigned int packetLength = frameData(packetPayload, unframedPacketLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendMpptData(unsigned char mpptDataIndex)
{
   const unsigned int unframedPacketLength = MPPT_DATA_LENGTH + CHECKSUM_LENGTH;
   unsigned char packetPayload[unframedPacketLength];
   packetPayload[0] = MPPT_DATA_ID;
   packetPayload[1] = mpptDataIndex;
   packetPayload[2] = static_cast<unsigned char>(vehicleData_.mpptData[mpptDataIndex].type);

   if (vehicleData_.mpptData[mpptDataIndex].type == MpptData::Helianthus)
   {
      packetPayload[3] = 0x1F;
   }
   else
   {
      packetPayload[3] = 0x07;
   }
   writeFloatIntoData(packetPayload, 4, vehicleData_.mpptData[mpptDataIndex].voltageIn);
   writeFloatIntoData(packetPayload, 8, vehicleData_.mpptData[mpptDataIndex].currentIn);
   writeFloatIntoData(packetPayload, 12, vehicleData_.mpptData[mpptDataIndex].voltageOut);
   writeFloatIntoData(packetPayload, 16, vehicleData_.mpptData[mpptDataIndex].currentOut);
   packetPayload[20] = static_cast<unsigned char>(vehicleData_.mpptData[mpptDataIndex].mode);

   addChecksum(packetPayload, MPPT_DATA_LENGTH);
   unsigned char packet[unframedPacketLength + FRAMING_LENGTH_INCREASE];
   unsigned int packetLength = frameData(packetPayload, unframedPacketLength, packet);
   sendData(packet, packetLength);
}

unsigned int TelemetryReporting::frameData(const unsigned char* dataToEncode,
      unsigned long length, unsigned char* frameData)
{
   unsigned int lengthOfFramedData =
      stuffData(dataToEncode, length, frameData);
   frameData[lengthOfFramedData++] = TERMINATING_BYTE;
   return lengthOfFramedData;
}

#define FINISH_BLOCK(X) \
{\
   *code_ptr = (X); \
   code_ptr = encodedData++; \
   code = 0x01; \
}

unsigned int TelemetryReporting::stuffData(const unsigned char* dataToEncode,
      unsigned long length, unsigned char* encodedData)
{
   unsigned int lengthOfEncodedData = length + 1;
   const unsigned char* end = dataToEncode + length;
   unsigned char* code_ptr = encodedData++;
   unsigned char code = 0x01;

   while (dataToEncode < end)
   {
      if (*dataToEncode == 0)
      {
         FINISH_BLOCK(code);
      }
      else
      {
         *encodedData++ = *dataToEncode;
         code++;
         if (code == 0xFF)
         {
            FINISH_BLOCK(code);
            lengthOfEncodedData++;
         }
      }
      dataToEncode++;
   }
   FINISH_BLOCK(code);
   return lengthOfEncodedData;
}
#undef FINISH_BLOCK

void TelemetryReporting::addChecksum(unsigned char* data, unsigned int length)
{
   unsigned short crc16 = CrcCalculator::calculateCrc16(data, length);
   data[length] = static_cast<unsigned char>(0xFF & crc16);
   data[length + 1] = static_cast<unsigned char>(0xFF & (crc16 >> 8));
}

void TelemetryReporting::writeFloatIntoData(unsigned char* data, int index, const float& value)
{
   FloatDataUnion floatDataUnion;
   floatDataUnion.floatData = value;
   data[index++] = floatDataUnion.charData[0];
   data[index++] = floatDataUnion.charData[1];
   data[index++] = floatDataUnion.charData[2];
   data[index] = floatDataUnion.charData[3];
}

void TelemetryReporting::sendData(const unsigned char* data, int length)
{
   for (int i = 0; i < length; ++i)
   {
      uart_.putc(data[i]);
   }
}
