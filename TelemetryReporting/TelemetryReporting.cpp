/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

// Solar car includes
#include <TelemetryReporting.h>
#include <VehicleData.h>

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
: bluetooth_(uartTx, uartRx)
, vehicleData_(vehicleData)
{
}

void TelemetryReporting::initTelemetryReporting()
{
   bluetooth_.baud(BLUETOOTH_BAUD_RATE);
   bluetooth_.format(BLUETOOTH_NUMBER_OF_BITS, BLUETOOTH_PARITY, BLUETOOTH_STOP_BIT);

  // For future use to receive data from chase car.
  // bluetooth_.attach(this, &TelemetryReporting::readBluetooth);

   bluetooth_.puts(GET_BLUETOOTH_STATUS);

   bluetooth_.puts("SET BT AUTH * 0001\n");

   bluetooth_.puts("SET BT PAGEMODE 3 3000 1\n");
}

void TelemetryReporting::transmitTelemetry()
{
   transmitVehicleStatus();
}

void TelemetryReporting::transmitVehicleStatus()
{
   sendFloatWithIdentifier("00", vehicleData_.driverSetSpeedRpm);
   sendFloatWithIdentifier("01", vehicleData_.driverSetCurrent);   
   
   sendFloatWithIdentifier("02", vehicleData_.vehicleVelocityKph);
   sendFloatWithIdentifier("03", vehicleData_.busCurrentA);
   sendFloatWithIdentifier("04", vehicleData_.busVoltage);
   sendFloatWithIdentifier("05", vehicleData_.motorVelocityRpm);
   sendFloatWithIdentifier("06", vehicleData_.motorVoltageReal);
   sendFloatWithIdentifier("07", vehicleData_.motorCurrentReal);
   sendFloatWithIdentifier("08", vehicleData_.backEmfImaginary);
   sendFloatWithIdentifier("09", vehicleData_.ipmHeatSinkTemp);
   sendFloatWithIdentifier("11", vehicleData_.dspBoardTemp);
   sendFloatWithIdentifier("12", vehicleData_.dcBusAmpHours);
   sendVariableWithIdentifier("14", vehicleData_.receivedErrorCount);
   sendVariableWithIdentifier("15", vehicleData_.transmittedErrorCount);

   sendVariableWithIdentifier("16", vehicleData_.cmuData[0].pcbTemperature);
   sendVariableWithIdentifier("17", vehicleData_.cmuData[0].cellTemperature);
   sendVariableWithIdentifier("18", vehicleData_.cmuData[0].cellVoltage[0]);
   sendVariableWithIdentifier("19", vehicleData_.cmuData[0].cellVoltage[1]);
   sendVariableWithIdentifier("20", vehicleData_.cmuData[0].cellVoltage[2]);
   sendVariableWithIdentifier("21", vehicleData_.cmuData[0].cellVoltage[3]);
   sendVariableWithIdentifier("22", vehicleData_.cmuData[0].cellVoltage[4]);
   sendVariableWithIdentifier("23", vehicleData_.cmuData[0].cellVoltage[5]);
   sendVariableWithIdentifier("24", vehicleData_.cmuData[0].cellVoltage[6]);
   sendVariableWithIdentifier("25", vehicleData_.cmuData[0].cellVoltage[7]);
   sendVariableWithIdentifier("26", vehicleData_.cmuData[1].pcbTemperature);
   sendVariableWithIdentifier("27", vehicleData_.cmuData[1].cellTemperature);
   sendVariableWithIdentifier("28", vehicleData_.cmuData[1].cellVoltage[0]);
   sendVariableWithIdentifier("29", vehicleData_.cmuData[1].cellVoltage[1]);
   sendVariableWithIdentifier("30", vehicleData_.cmuData[1].cellVoltage[2]);
   sendVariableWithIdentifier("31", vehicleData_.cmuData[1].cellVoltage[3]);
   sendVariableWithIdentifier("32", vehicleData_.cmuData[1].cellVoltage[4]);
   sendVariableWithIdentifier("33", vehicleData_.cmuData[1].cellVoltage[5]);
   sendVariableWithIdentifier("34", vehicleData_.cmuData[1].cellVoltage[6]);
   sendVariableWithIdentifier("35", vehicleData_.cmuData[1].cellVoltage[7]);
   sendVariableWithIdentifier("36", vehicleData_.cmuData[2].pcbTemperature);
   sendVariableWithIdentifier("37", vehicleData_.cmuData[2].cellTemperature);
   sendVariableWithIdentifier("38", vehicleData_.cmuData[2].cellVoltage[0]);
   sendVariableWithIdentifier("39", vehicleData_.cmuData[2].cellVoltage[1]);
   sendVariableWithIdentifier("40", vehicleData_.cmuData[2].cellVoltage[2]);
   sendVariableWithIdentifier("41", vehicleData_.cmuData[2].cellVoltage[3]);
   sendVariableWithIdentifier("42", vehicleData_.cmuData[2].cellVoltage[4]);
   sendVariableWithIdentifier("43", vehicleData_.cmuData[2].cellVoltage[5]);
   sendVariableWithIdentifier("44", vehicleData_.cmuData[2].cellVoltage[6]);
   sendVariableWithIdentifier("45", vehicleData_.cmuData[2].cellVoltage[7]);
   sendVariableWithIdentifier("46", vehicleData_.cmuData[3].pcbTemperature);
   sendVariableWithIdentifier("47", vehicleData_.cmuData[3].cellTemperature);
   sendVariableWithIdentifier("48", vehicleData_.cmuData[3].cellVoltage[0]);
   sendVariableWithIdentifier("49", vehicleData_.cmuData[3].cellVoltage[1]);
   sendVariableWithIdentifier("50", vehicleData_.cmuData[3].cellVoltage[2]);
   sendVariableWithIdentifier("51", vehicleData_.cmuData[3].cellVoltage[3]);
   sendVariableWithIdentifier("52", vehicleData_.cmuData[3].cellVoltage[4]);
   sendVariableWithIdentifier("53", vehicleData_.cmuData[3].cellVoltage[5]);
   sendVariableWithIdentifier("54", vehicleData_.cmuData[3].cellVoltage[6]);
   sendVariableWithIdentifier("55", vehicleData_.cmuData[3].cellVoltage[7]);   
   sendVariableWithIdentifier("56", vehicleData_.batteryVoltage);
   sendVariableWithIdentifier("57", vehicleData_.batteryCurrent);
   sendVariableWithIdentifier("58", vehicleData_.batteryVoltageThresholdRising);
   sendVariableWithIdentifier("59", vehicleData_.batteryVoltageThresholdFalling);
}

int TelemetryReporting::fixPointApproixmation(const float input)
{
   return static_cast<int>(input * 1000.0f); 
}

void TelemetryReporting::sendVariable(const int input)
{
   unsigned int bitsToSend = static_cast<unsigned int>(input);

   for (int i = 0; i < sizeof(bitsToSend); ++i)
   {
      bluetooth_.putc(bitsToSend & 0xFF);
      bitsToSend = bitsToSend >> 8;
   }
}

void TelemetryReporting::sendFloatWithIdentifier(const char* identifier, const float input)
{
   char identifierString[50];
   sprintf(identifierString, "#%s", identifier);
   bluetooth_.puts(identifierString);
   int dataToSend = fixPointApproixmation(input);
   sendVariable(dataToSend);
   bluetooth_.putc('\n');
}

template <typename T>
void TelemetryReporting::sendVariableWithIdentifier(const char* identifier, const T input)
{
   char identifierString[50];
   sprintf(identifierString, "#%s", identifier);
   bluetooth_.puts(identifierString);  
   sendVariable(static_cast<int>(input));
   bluetooth_.putc('\n');
}
