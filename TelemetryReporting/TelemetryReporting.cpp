/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

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
   unsigned int BLUETOOTH_BAUD_RATE = 115200;
   unsigned int BLUETOOTH_NUMBER_OF_BITS = 8;
   unsigned int BLUETOOTH_STOP_BIT = 1;
   SerialBase::Parity BLUETOOTH_PARITY = SerialBase::None;

   const char GET_BLUETOOTH_STATUS[] = "AT\n";

   // These lengths only include the data. Not the checksum
   const unsigned int KEY_DRIVER_CONTROL_LENGTH = 21;
   const unsigned int DRIVER_CONTROL_DETAILS_LENGTH = 29;
   const unsigned int FAULT_LENGTH = 7;
   const unsigned int BATTERY_DATA_LENGTH = 18;
   const unsigned int CMU_DATA_LENGTH = 43;
   const unsigned int CHECKSUM_LENGTH = 2;

   const unsigned int FRAMING_LENGTH_INCREASE = 5;
   const unsigned char TERMINATING_BYTE = 0x00;

   // Packet IDs
   const unsigned char KEY_DRIVER_CONTROL_ID = 0x01;
   const unsigned char DRIVER_CONTROL_DETAILS_ID = 0x02;
   const unsigned char FAULT_ID = 0x03;
   const unsigned char BATTERY_DATA_ID = 0x04;
   const unsigned char CMU_DATA_ID = 0x05;
}

TelemetryReporting::TelemetryReporting(PinName uartTx,
                                       PinName uartRx,
                                       VehicleData& vehicleData)
: uart_(uartTx, uartRx)
, vehicleData_(vehicleData)
{
}

void TelemetryReporting::initTelemetryReporting()
{
#ifdef BLUETOOTH
   uart_.baud(BLUETOOTH_BAUD_RATE);
   uart_.format(BLUETOOTH_NUMBER_OF_BITS, BLUETOOTH_PARITY, BLUETOOTH_STOP_BIT);

   uart_.puts(GET_BLUETOOTH_STATUS);

   uart_.puts("SET BT AUTH * 0001\n");

   uart_.puts("SET BT PAGEMODE 3 3000 1\n");
#endif
}

void TelemetryReporting::transmitTelemetry()
{
   // todo, send at different frequencies
   sendKeyDriverControlTelemetry();
   sendDriverControlDetails();
   sendFaults();
   sendBatteryData();
   sendCmuData(0);
   sendCmuData(1);
   sendCmuData(2);
   sendCmuData(3);
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
   packetPayload[1] = static_cast<unsigned char>(0xFF & vehicleData_.errorFlags);
   packetPayload[2] = static_cast<unsigned char>(0xFF & vehicleData_.limitFlags);
   packetPayload[3] = static_cast<unsigned char>(0xFF & vehicleData_.bmuStatusFlagsExtended);
   packetPayload[4] = static_cast<unsigned char>(0xFF & (vehicleData_.bmuStatusFlagsExtended >> 8));
   packetPayload[5] = vehicleData_.receivedErrorCount;
   packetPayload[6] = vehicleData_.transmittedErrorCount;

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
