/*-------------------------------------------------------
   Made for the dashboard OLED displays
   By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
   Copyright (c) 2015 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <CcsDefines.h>
#include <CharDisplay.h>
#include <Dashboard.h>
#include <VehicleData.h>

namespace
{
   const int HELIANTHUS_MPPT_INDEX = 3;
   const char MPPT_STATIC_MODE[] = "Static";
   const char MPPT_DYNAMIC_MODE[] = "Dynamic";
   const char MPPT_IVTRACE_MODE[] = "IvTrace";
   const char MPPT_OPEN_MODE[] = "Open";
   const char MPPT_INVALID_MODE[] = "Invalid";

   const char BMU_PRECHARGE_ERROR[] = "Error";
   const char BMU_PRECHARGE_IDLE[] = "Idle";
   const char BMU_PRECHARGE_ENABLE_PACK[] = "Enable Pack";
   const char BMU_PRECHARGE_MEASURE[] = "Measure";
   const char BMU_PRECHARGE_PRECHARGE[] = "Precharge";
   const char BMU_PRECHARGE_RUN[] = "Run";
   const char BMU_PRECHARGE_INVALID[] = "Invalid";

   const char* getMpptModeString(MpptData::Mode mode)
   {
      switch(mode)
      {
      case MpptData::Static:
         return MPPT_STATIC_MODE;
      case MpptData::Dynamic:
         return MPPT_DYNAMIC_MODE;
      case MpptData::IvTrace:
         return MPPT_IVTRACE_MODE;
      case MpptData::Open:
         return MPPT_OPEN_MODE;
      default:
         return MPPT_INVALID_MODE;
      }
   }

   const char* getPrechargeStateString(VehicleData::PrechargeStates state)
   {
      switch(state)
      {
      case VehicleData::Error:
         return BMU_PRECHARGE_ERROR;
      case VehicleData::Idle:
         return BMU_PRECHARGE_IDLE;
      case VehicleData::EnablePack:
         return BMU_PRECHARGE_ENABLE_PACK;
      case VehicleData::Measure:
         return BMU_PRECHARGE_MEASURE;
      case VehicleData::Precharge:
         return BMU_PRECHARGE_PRECHARGE;
      case VehicleData::Run:
         return BMU_PRECHARGE_RUN;
      default:
         return BMU_PRECHARGE_INVALID;
      }
   }
}

Dashboard::Dashboard(
      const PinName& mosi,
      const PinName& miso,
      const PinName& clock,
      const PinName& chipSelectChar1Display,
      const PinName& chipSelectChar2Display,
      const PinName& chipSelectChar3Display,
      const VehicleData& vehicleData)
: serialCommunication_(mosi, miso, clock)
, charDisplay1_(&serialCommunication_, chipSelectChar1Display)
, charDisplay2_(&serialCommunication_, chipSelectChar2Display)
, charDisplay3_(&serialCommunication_, chipSelectChar3Display)
, timer_(0)
, vehicleData_(vehicleData)
{
}

void Dashboard::initAll()
{
   wait_ms(1); //wait for power to stabalize on the displays
   charDisplay1_.init();
   charDisplay2_.init();
   charDisplay3_.init();
   wait_ms(1); //Wait to make sure it is all completed.
}

void Dashboard::updateDashboard()
{
   switch(timer_)
   {
   case 0:
      updateDisplay1();
      break;
   case 1:
      updateDisplay2();
      break;
   case 2:
      updateDisplay3();
      break;
   }

   if (timer_ == 2)
   {
      timer_ = 0;
   }
   else
   {
      timer_++;
   }
}

void Dashboard::updateDisplay1()
{
   char message1[21];
   char message2[21];
   char message3[21];
   char message4[21];
   memset(message1, ' ', 20);
   memset(message2, ' ', 20);
   memset(message3, ' ', 20);
   memset(message4, ' ', 20);

   const float driverSetSpeedKph = vehicleData_.driverSetSpeedRpm * CcsDefines::RPM_TO_KPH_CONVERSION;
   sprintf(message1, "Set Speed:  %3.0f km/h", driverSetSpeedKph);
   const float vehicleVelocityKph = vehicleData_.vehicleVelocity * CcsDefines::MPS_KPH_CONVERSION;
   sprintf(message2, "ATCL Speed: %3.0f km/h", vehicleVelocityKph);
   sprintf(message3, "Set Current: %4.1f A ", vehicleData_.driverSetCurrent);
   sprintf(message4, "Odometer:  %6.1f km", vehicleData_.odometer / 1000.0f);

   charDisplay1_.setLine(1);
   charDisplay1_.writeMessage(message1,20);

   charDisplay1_.setLine(2);
   charDisplay1_.writeMessage(message2, 20);

   charDisplay1_.setLine(3);
   charDisplay1_.writeMessage(message3, 20);

   charDisplay1_.setLine(4);
   charDisplay1_.writeMessage(message4, 20);
}

void Dashboard::updateDisplay2()
{
   char message1[21];
   char message2[21];
   char message3[21];
   char message4[21];
   memset(message1, ' ', 20);
   memset(message2, ' ', 20);
   memset(message3, ' ', 20);
   memset(message4, ' ', 20);

   sprintf(message1, "Bat Current: %5.1f A", vehicleData_.batteryCurrent);
   sprintf(message2, "Bus Voltage: %5.1f V", vehicleData_.busVoltage);
   const float power = vehicleData_.busVoltage * vehicleData_.busCurrent;
   sprintf(message3, "Power Draw:  %5.1f W", power);
   sprintf(message4, "High Voltage:  %s",
      (vehicleData_.prechargeState >= 4)?"On":"Off");

   charDisplay2_.setLine(1);
   charDisplay2_.writeMessage(message1,20);

   charDisplay2_.setLine(2);
   charDisplay2_.writeMessage(message2, 20);

   charDisplay2_.setLine(3);
   charDisplay2_.writeMessage(message3, 20);

   charDisplay2_.setLine(4);
   charDisplay2_.writeMessage(message4, 20);
}

void Dashboard::updateDisplay3()
{
   char message1[21];
   char message2[21];
   char message3[21];
   char message4[21];
   memset(message1, ' ', 20);
   memset(message2, ' ', 20);
   memset(message3, ' ', 20);
   memset(message4, ' ', 20);

   const MpptData::Mode commandedMpptMode = vehicleData_.dynamicModeActivated ?
      MpptData::Dynamic : MpptData::Static;
   sprintf(message1, "Mode cmd:    %s", getMpptModeString(commandedMpptMode));
   const MpptData::Mode reportedMpptMode = vehicleData_.mpptData[HELIANTHUS_MPPT_INDEX].mode;
   sprintf(message2, "Mode rpt:    %s", getMpptModeString(reportedMpptMode));
   sprintf(message3, "BMU State: %s", getPrechargeStateString(vehicleData_.prechargeState));
   const bool majorMcError = vehicleData_.errorFlags & CcsDefines::MAJOR_MC_ERROR_MASK;
   sprintf(message4, "%s", majorMcError ? "MC state:    error!":"MC state:    Okay");

   charDisplay3_.setLine(1);
   charDisplay3_.writeMessage(message1,20);

   charDisplay3_.setLine(2);
   charDisplay3_.writeMessage(message2, 20);

   charDisplay3_.setLine(3);
   charDisplay3_.writeMessage(message3, 20);

   charDisplay3_.setLine(4);
   charDisplay3_.writeMessage(message4, 20);
}

void Dashboard::displayTest(CharDisplay& display)
{
    char message1[] = "ACTL Volt:  XXX.X  V";
    char message2[] = "ACTL Amp:   XXX.X  A";
    char message3[] = "Set  Amp:   XXX.X  A";
    char message4[] = "Set Speed:  XXX km/h";

    display.setLine(1);
    display.writeMessage(message1,20);

    display.setLine(2);
    display.writeMessage(message2, 20);

    display.setLine(3);
    display.writeMessage(message3, 20);

    display.setLine(4);
    display.writeMessage(message4, 20);
}
