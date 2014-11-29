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
   sendFloatWithIdentifier("10", vehicleData_.dspBoardTemp);
   sendFloatWithIdentifier("11", vehicleData_.dcBusAmpHours);
   sendFloatWithIdentifier("12", static_cast<float>(vehicleData_.receivedErrorCount));
   sendFloatWithIdentifier("13", static_cast<float>(vehicleData_.transmittedErrorCount));

   sendFloatWithIdentifier("14", vehicleData_.cmuData[0].pcbTemperature);
   sendFloatWithIdentifier("15", vehicleData_.cmuData[0].cellTemperature);
   sendFloatWithIdentifier("16", vehicleData_.cmuData[0].cellVoltage[0]);
   sendFloatWithIdentifier("17", vehicleData_.cmuData[0].cellVoltage[1]);
   sendFloatWithIdentifier("18", vehicleData_.cmuData[0].cellVoltage[2]);
   sendFloatWithIdentifier("19", vehicleData_.cmuData[0].cellVoltage[3]);
   sendFloatWithIdentifier("20", vehicleData_.cmuData[0].cellVoltage[4]);
   sendFloatWithIdentifier("21", vehicleData_.cmuData[0].cellVoltage[5]);
   sendFloatWithIdentifier("22", vehicleData_.cmuData[0].cellVoltage[6]);
   sendFloatWithIdentifier("23", vehicleData_.cmuData[0].cellVoltage[7]);
   sendFloatWithIdentifier("24", vehicleData_.cmuData[1].pcbTemperature);
   sendFloatWithIdentifier("25", vehicleData_.cmuData[1].cellTemperature);
   sendFloatWithIdentifier("26", vehicleData_.cmuData[1].cellVoltage[0]);
   sendFloatWithIdentifier("27", vehicleData_.cmuData[1].cellVoltage[1]);
   sendFloatWithIdentifier("28", vehicleData_.cmuData[1].cellVoltage[2]);
   sendFloatWithIdentifier("29", vehicleData_.cmuData[1].cellVoltage[3]);
   sendFloatWithIdentifier("30", vehicleData_.cmuData[1].cellVoltage[4]);
   sendFloatWithIdentifier("31", vehicleData_.cmuData[1].cellVoltage[5]);
   sendFloatWithIdentifier("32", vehicleData_.cmuData[1].cellVoltage[6]);
   sendFloatWithIdentifier("33", vehicleData_.cmuData[1].cellVoltage[7]);
   sendFloatWithIdentifier("34", vehicleData_.cmuData[2].pcbTemperature);
   sendFloatWithIdentifier("35", vehicleData_.cmuData[2].cellTemperature);
   sendFloatWithIdentifier("36", vehicleData_.cmuData[2].cellVoltage[0]);
   sendFloatWithIdentifier("37", vehicleData_.cmuData[2].cellVoltage[1]);
   sendFloatWithIdentifier("38", vehicleData_.cmuData[2].cellVoltage[2]);
   sendFloatWithIdentifier("39", vehicleData_.cmuData[2].cellVoltage[3]);
   sendFloatWithIdentifier("40", vehicleData_.cmuData[2].cellVoltage[4]);
   sendFloatWithIdentifier("41", vehicleData_.cmuData[2].cellVoltage[5]);
   sendFloatWithIdentifier("42", vehicleData_.cmuData[2].cellVoltage[6]);
   sendFloatWithIdentifier("43", vehicleData_.cmuData[2].cellVoltage[7]);
   sendFloatWithIdentifier("44", vehicleData_.cmuData[3].pcbTemperature);
   sendFloatWithIdentifier("45", vehicleData_.cmuData[3].cellTemperature);
   sendFloatWithIdentifier("46", vehicleData_.cmuData[3].cellVoltage[0]);
   sendFloatWithIdentifier("47", vehicleData_.cmuData[3].cellVoltage[1]);
   sendFloatWithIdentifier("48", vehicleData_.cmuData[3].cellVoltage[2]);
   sendFloatWithIdentifier("49", vehicleData_.cmuData[3].cellVoltage[3]);
   sendFloatWithIdentifier("50", vehicleData_.cmuData[3].cellVoltage[4]);
   sendFloatWithIdentifier("51", vehicleData_.cmuData[3].cellVoltage[5]);
   sendFloatWithIdentifier("52", vehicleData_.cmuData[3].cellVoltage[6]);
   sendFloatWithIdentifier("53", vehicleData_.cmuData[3].cellVoltage[7]);   
   sendFloatWithIdentifier("54", vehicleData_.batteryVoltage);
   sendFloatWithIdentifier("55", vehicleData_.batteryCurrent);
   sendFloatWithIdentifier("56", static_cast<float>(vehicleData_.batteryVoltageThresholdRising));
   sendFloatWithIdentifier("57", static_cast<float>(vehicleData_.batteryVoltageThresholdFalling));
}

void TelemetryReporting::sendFloatWithIdentifier(const char* identifier, const float input)
{
   char identifierString[50];
   bluetooth_.puts(identifierString);
   sprintf(identifierString, "#%s%.3f", identifier, input);
   bluetooth_.putc('\n');
}

template <typename T>
void TelemetryReporting::sendVariableWithIdentifier(const char* identifier, const T input)
{
   char identifierString[50];
   sprintf(identifierString, "#%s%d", identifier, input);
   bluetooth_.puts(identifierString);  
   bluetooth_.putc('\n');
}
