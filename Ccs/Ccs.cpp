/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs on for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>

#include <Ccs.h>

namespace
{
   const PinName& PIN_DISPLAY_MOSI = p5;
   const PinName& PIN_DISPLAY_MISO = p6;
   const PinName& PIN_DISPLAY_CLOCK = p7;
   const PinName& PIN_DISPLAY_CHAR_CS = p8;

   const PinName& CAN_MPPT_RD = p9;
   const PinName& CAN_MPPT_TD = p10;

   const PinName& PIN_DEADMAN_INPUT = p14;
   const PinName& PIN_VELOCITY_INPUT = p15;
   const PinName& PIN_CURRENT_INPUT = p16;

   const PinName& PIN_CAR_DIRECTION_INPUT = p17;
   const PinName& PIN_LIGHTS_HAZARDS_INPUT = p18;
   const PinName& PIN_LIGHTS_RIGHT_BLINKER_INPUT = p19;
   const PinName& PIN_LIGHTS_LEFT_BLINKER_INPUT = p20;
   const PinName& PIN_LIGHTS_BRAKE_INPUT = p21;

   const PinName& PIN_LIGHTS_BRAKE = p24;
   const PinName& PIN_LIGHTS_LEFT_BLINKER = p25;
   const PinName& PIN_LIGHTS_RIGHT_BLINKER = p26;

   const PinName& PIN_BLUETOOTH_UART_RX = p27;
   const PinName& PIN_BLUETOOTH_UART_TX = p28;

   const PinName& CAN_TD = p29;
   const PinName& CAN_RD = p30;
}

Ccs::Ccs()
: vehicleData_()
, lights_(PIN_LIGHTS_BRAKE,
          PIN_LIGHTS_LEFT_BLINKER,
          PIN_LIGHTS_RIGHT_BLINKER,
          vehicleData_)
, canInterface_(CAN_TD,
                CAN_RD,
                CAN_MPPT_TD,
                CAN_MPPT_RD,
                vehicleData_)
, driverControl_(PIN_DEADMAN_INPUT,
                 PIN_LIGHTS_HAZARDS_INPUT,
                 PIN_LIGHTS_RIGHT_BLINKER_INPUT,
                 PIN_LIGHTS_LEFT_BLINKER_INPUT,
                 PIN_LIGHTS_BRAKE_INPUT,
                 PIN_CURRENT_INPUT,
                 PIN_VELOCITY_INPUT,
                 PIN_CAR_DIRECTION_INPUT,
                 vehicleData_)
, dashboard_(PIN_DISPLAY_MOSI,
             PIN_DISPLAY_MISO,
             PIN_DISPLAY_CLOCK,
             PIN_DISPLAY_CHAR_CS,
             vehicleData_)
, telemetryReporting_(PIN_BLUETOOTH_UART_TX,
                      PIN_BLUETOOTH_UART_RX,
                      vehicleData_)
, ledBmuErrorOutputService_(vehicleData_)
, telemetryTimer_(0)
, displayTimer_(0)
{
}

void Ccs::initalizeActions()
{
   driverControl_.initializeDriverControls();
   canInterface_.initInterface();
   dashboard_.initAll();
   lights_.initalizeLights();
   telemetryReporting_.initTelemetryReporting();
}

void Ccs::performActions()
{
   driverControl_.readDriverControls();
   canInterface_.sendCanData();
   lights_.updateLights();

   //At 120 ms this would be sending telemetry at the rate of every 5 seconds
   if(telemetryTimer_ >= 42)
   {
      telemetryReporting_.transmitTelemetry();
      ledBmuErrorOutputService_.updateLeds();
      telemetryTimer_ = 0;
   }

   //At 120 ms this would be updating the display at the rate of 4.1 Hz
   if(displayTimer_ >= 2)
   {
      dashboard_.displayDashboard();
      displayTimer_ = 0;
   }

   telemetryTimer_++;
   displayTimer_++;
}
