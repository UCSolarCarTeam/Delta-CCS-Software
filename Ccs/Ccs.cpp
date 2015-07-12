/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <mbed.h>

#include <Ccs.h>

namespace
{
   const PinName& PIN_DISPLAY_MOSI = p5;
   const PinName& PIN_DISPLAY_MISO = p6;
   const PinName& PIN_DISPLAY_CLOCK = p7;
   const PinName& PIN_DISPLAY_CHAR_1_CS = p8;

   const PinName& CAN_MPPT_RD = p9;
   const PinName& CAN_MPPT_TD = p10;

   const PinName& PIN_DISPLAY_CHAR_3_CS = p11;
   const PinName& PIN_DISPLAY_CHAR_2_CS = p12;

   const PinName& MPPT_MODE_SWITCH = p13;
   const PinName& PIN_DEADMAN_INPUT = p14;
   const PinName& PIN_REGEN_INPUT = p15;
   const PinName& PIN_CURRENT_INPUT = p16;

   const PinName& PIN_CAR_DIRECTION_INPUT = p17;
   const PinName& PIN_LIGHTS_HAZARDS_INPUT = p18;
   const PinName& PIN_LIGHTS_RIGHT_BLINKER_INPUT = p19;
   const PinName& PIN_LIGHTS_LEFT_BLINKER_INPUT = p20;
   const PinName& PIN_LIGHTS_BRAKE_INPUT = p21;

   const PinName& PIN_LIGHTS_BRAKE_OUT = p22;
   const PinName& PIN_LIGHTS_LEFT_BLINKER_OUT = p23;
   const PinName& PIN_LIGHTS_RIGHT_BLINKER_OUT = p24;

   const PinName& RESET_IN = p25;

   const PinName& PIN_RADIO_UART_RX = p27;
   const PinName& PIN_RADIO_UART_TX = p28;

   const PinName& CAN_TD = p29;
   const PinName& CAN_RD = p30;

   // These are the USB pins 31 and 32
   const PinName& ARRAY_ON_INPUT = P0_29;
   const PinName& HIGH_VOLTAGE_INPUT = P0_30;
}

Ccs::Ccs()
: vehicleData_()
, lights_(
   PIN_LIGHTS_BRAKE_OUT,
   PIN_LIGHTS_LEFT_BLINKER_OUT,
   PIN_LIGHTS_RIGHT_BLINKER_OUT,
   vehicleData_)
, motorControllerCan_(
   CAN_TD,
   CAN_RD,
   RESET_IN,
   vehicleData_)
, mpptCan_(
   CAN_MPPT_TD,
   CAN_MPPT_RD,
   vehicleData_)
, driverControl_(
   PIN_DEADMAN_INPUT,
   PIN_LIGHTS_HAZARDS_INPUT,
   PIN_LIGHTS_RIGHT_BLINKER_INPUT,
   PIN_LIGHTS_LEFT_BLINKER_INPUT,
   PIN_LIGHTS_BRAKE_INPUT,
   PIN_CURRENT_INPUT,
   PIN_REGEN_INPUT,
   PIN_CAR_DIRECTION_INPUT,
   MPPT_MODE_SWITCH,
   ARRAY_ON_INPUT,
   HIGH_VOLTAGE_INPUT,
   vehicleData_)
, dashboard_(
   PIN_DISPLAY_MOSI,
   PIN_DISPLAY_MISO,
   PIN_DISPLAY_CLOCK,
   PIN_DISPLAY_CHAR_1_CS,
   PIN_DISPLAY_CHAR_2_CS,
   PIN_DISPLAY_CHAR_3_CS,
   vehicleData_)
, telemetryReporting_(
   PIN_RADIO_UART_TX,
   PIN_RADIO_UART_RX,
   vehicleData_)
, ledBmuErrorOutputService_(
   vehicleData_)
, telemetryTimer_(0)
, displayTimer_(0)
{
}

void Ccs::initalizeActions()
{
   driverControl_.initializeDriverControls();
   motorControllerCan_.initInterface();
   mpptCan_.initInterface();
   dashboard_.initAll();
   lights_.initalizeLights();
   telemetryReporting_.initTelemetryReporting();
}

void Ccs::performActions()
{
   driverControl_.readDriverControls();
   motorControllerCan_.sendCanData();
   mpptCan_.sendCanData();
   lights_.updateLights();

   //At 60 ms this would be sending telemetry at the rate of every 0.5 seconds
   if(telemetryTimer_ >= 8)
   {
      telemetryReporting_.transmitTelemetry();
      ledBmuErrorOutputService_.updateLeds();
      telemetryTimer_ = 0;
   }

   //At 60 ms this would be updating the display at the rate of 2.5 Hz
   if(displayTimer_ >= 2)
   {
      dashboard_.updateDashboard();
      displayTimer_ = 0;
   }

   telemetryTimer_++;
   displayTimer_++;
}
