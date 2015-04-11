/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#pragma once

// Mbed includes
#include <mbed.h>

// Solar car includes
class VehicleData;

class TelemetryReporting
{
public:
   TelemetryReporting(PinName uartTx,
                      PinName uartRx,
                      VehicleData& vehicleData);

   void initTelemetryReporting();
   void transmitTelemetry();

private:
   void sendKeyDriverControlTelemetry();
   void sendDriverControlDetails();
   void sendFaults();
   void sendBatteryData();
   void sendCmuData(unsigned char cmuDataIndex);

   // Will return length of framed data
   unsigned int frameData(const unsigned char* dataToEncode,
         unsigned long length, unsigned char* frameData);
   // Will return length of encoded data
   unsigned int stuffData(const unsigned char* dataToEncode,
         unsigned long length, unsigned char* encodedData);
   // Add checksum into data at index length and length + 1
   void addChecksum(unsigned char* data, unsigned int length);

   void writeFloatIntoData(unsigned char* data, int index, const float& value);
   void sendData(const unsigned char* data, int length);

private:
   RawSerial uart_;
   VehicleData& vehicleData_;
};
