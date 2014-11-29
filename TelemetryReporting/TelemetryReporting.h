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
   void transmitVehicleStatus();
   
   void sendVariable(const int input);
   void sendFloatWithIdentifier(const char* identifier, const float input);

   template<typename T>
   void sendVariableWithIdentifier(const char* identifier, const T input);

private:
   RawSerial bluetooth_;
   VehicleData& vehicleData_;
};
