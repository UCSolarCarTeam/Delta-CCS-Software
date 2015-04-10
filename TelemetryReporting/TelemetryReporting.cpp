/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#define BLUETOOTH

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

  // For future use to receive data from chase car.
  // uart_.attach(this, &TelemetryReporting::readBluetooth);

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
   const int packetPayloadLength = 21;
   unsigned char packetPayload[packetPayloadLength];
   packetPayload[0] = 0x01;
   writeFloatIntoData(packetPayload, 1, vehicleData_.actualSpeedRpm); //todo send m/s
   writeFloatIntoData(packetPayload, 5, vehicleData_.driverSetCurrentA);
   writeFloatIntoData(packetPayload, 9, vehicleData_.busCurrentA);
   writeFloatIntoData(packetPayload, 13, vehicleData_.busVoltage);
   writeFloatIntoData(packetPayload, 17, vehicleData_.vehicleVelocityKph);

   unsigned char packet[50];
   unsigned int packetLength = frameData(packetPayload, packetPayloadLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendDriverControlDetails()
{
   const int packetPayloadLength = 29;
   unsigned char packetPayload[packetPayloadLength];
   packetPayload[0] = 0x02;
   writeFloatIntoData(packetPayload, 1, vehicleData_.motorVelocityRpm);
   writeFloatIntoData(packetPayload, 5, vehicleData_.motorVoltageReal);
   writeFloatIntoData(packetPayload, 9, vehicleData_.motorCurrentReal);
   writeFloatIntoData(packetPayload, 13, vehicleData_.backEmfImaginary);
   writeFloatIntoData(packetPayload, 17, vehicleData_.dspBoardTemp);
   writeFloatIntoData(packetPayload, 21, vehicleData_.dcBusAmpHours);
   writeFloatIntoData(packetPayload, 25, vehicleData_.odometer);

   unsigned char packet[50];
   unsigned int packetLength = frameData(packetPayload, packetPayloadLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendFaults()
{
   const int packetPayloadLength = 7;
   unsigned char packetPayload[packetPayloadLength];
   packetPayload[0] = 0x03;
   packetPayload[1] = static_cast<unsigned char>(0xFF & vehicleData_.errorFlags);
   packetPayload[2] = static_cast<unsigned char>(0xFF & vehicleData_.limitFlags);
   packetPayload[3] = static_cast<unsigned char>(0xFF & vehicleData_.bmuStatusFlagsExtended);
   packetPayload[4] = static_cast<unsigned char>(0xFF & (vehicleData_.bmuStatusFlagsExtended >> 1));
   packetPayload[5] = vehicleData_.receivedErrorCount;
   packetPayload[6] = vehicleData_.transmittedErrorCount;
   unsigned char packet[10];
   unsigned int packetLength = frameData(packetPayload, packetPayloadLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendBatteryData()
{
   const int packetPayloadLength = 18;
   unsigned char packetPayload[packetPayloadLength];
   packetPayload[0] = 0x04;
   writeFloatIntoData(packetPayload, 1, vehicleData_.batteryVoltage);
   writeFloatIntoData(packetPayload, 5, vehicleData_.batteryCurrent);
   writeFloatIntoData(packetPayload, 9, vehicleData_.packStateOfChargePercentage);
   writeFloatIntoData(packetPayload, 13, vehicleData_.balancePackStateOfCharge);
   packetPayload[17] = static_cast<unsigned char>(vehicleData_.secondaryBatteryUnderVoltage);
   unsigned char packet[25];
   unsigned int packetLength = frameData(packetPayload, packetPayloadLength, packet);
   sendData(packet, packetLength);
}

void TelemetryReporting::sendCmuData(unsigned char cmuDataIndex)
{
   const int packetPayloadLength = 43;
   unsigned char packetPayload[packetPayloadLength];
   packetPayload[0] = 0x05;
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
   unsigned char packet[50];
   unsigned int packetLength = frameData(packetPayload, packetPayloadLength, packet);
   sendData(packet, packetLength);
}

unsigned int TelemetryReporting::frameData(const unsigned char* dataToEncode,
      unsigned long length, unsigned char* frameData)
{
   unsigned int lengthOfFramedData =
      stuffData(dataToEncode, length, frameData);
   frameData[lengthOfFramedData++] = 0x00;
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

void TelemetryReporting::writeFloatIntoData(unsigned char* data, int index, float& value)
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
