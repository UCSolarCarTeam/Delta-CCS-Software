/*-------------------------------------------------------
   Made for the dashboard OLED displays
   By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
   Copyright (c) 2015 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#include <CcsDefines.h>
#include <CharDisplay.h>
#include <Dashboard.h>
#include <VehicleData.h>

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

   const float driverSetSpeedKph = vehicleData_.driverSetSpeedRpm * CcsDefines::RPM_TO_KPH_CONVERSION;
   sprintf(message1, "Set Speed:  %3.1f km/h", driverSetSpeedKph);
   const float vehicleVelocityKph = vehicleData_.vehicleVelocity * CcsDefines::MPS_KPH_CONVERSION;
   sprintf(message2, "ATCL Speed: %3.0f km/h", vehicleVelocityKph);
   sprintf(message3, "Set Current %4.1f A", vehicleData_.driverSetCurrent);
   sprintf(message4, "Odometer %4.1f km", vehicleData_.odometer / 1000.0f);

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

   sprintf(message1, "Bus Current %4.1f A ", vehicleData_.busCurrent);
   sprintf(message2, "Bus Voltage %4.1f V", vehicleData_.busVoltage);
   const float power = vehicleData_.busVoltage * vehicleData_.busCurrent;
   sprintf(message3, "Power   %4.1f  A", power);
   sprintf(message4, "FTW mode enabled!");

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

   const bool majorBmuError = vehicleData_.bmuStatusFlagsExtended & CcsDefines::MAJOR_BMU_ERROR_MASK;
   sprintf(message1, "%s", majorBmuError?"BMU error detected!":"BMU okay");
   const bool majorMcError = vehicleData_.errorFlags & CcsDefines::MAJOR_MC_ERROR_MASK;
   sprintf(message2, "%s", majorMcError?"MC error detected!":"MC okay");
   sprintf(message3, "MPPT status: N/A");
   sprintf(message4, "High Voltage: %s",
      (vehicleData_.prechargeState >= 4)?"Enabled":"Disabled");

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
