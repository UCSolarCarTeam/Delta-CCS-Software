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
   void sendMpptData(unsigned char mpptDataIndex);

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
   int timer_;
};
