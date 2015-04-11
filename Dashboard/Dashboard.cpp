#include <CharDisplay.h>
#include <VehicleData.h>

#include <Dashboard.h>

Dashboard::Dashboard(PinName mosi, PinName miso, PinName clock, PinName chipSelectCharDisplay,
            PinName chipSelectGraphicDisplay, PinName graphicDisplayDataSignal,
            PinName graphicDisplayReset,
            VehicleData& vehicleData)
: serialCommunication_(mosi, miso, clock)
, charDisplay_(&serialCommunication_, chipSelectCharDisplay)
, vehicleData_(vehicleData)
{
}

void Dashboard::initAll()
{
  wait_ms(1); //wait for power to stabalize on the displays
  charDisplay_.init();
  wait_ms(1); //Wait to make sure it is all completed.
}

void Dashboard::displayDashboard()
{
   char message1[21];
   char message2[21];
   char message3[21];
   char message4[21];

   sprintf(message1, "ATCL %4.1f V %4.1f A ", vehicleData_.busVoltage, vehicleData_.busCurrentA);
   sprintf(message2, "ATCL Speed: %3.0f km/h", vehicleData_.vehicleVelocityKph);
   sprintf(message3, "Set  Amp:   %4.1f  A", vehicleData_.driverSetCurrentA);
   sprintf(message4, "Set Speed:  %3.1f km/h", vehicleData_.driverSetSpeedKph);

   charDisplay_.setLine(1);
   charDisplay_.writeMessage(message1,20);

   charDisplay_.setLine(2);
   charDisplay_.writeMessage(message2, 20);

   charDisplay_.setLine(3);
   charDisplay_.writeMessage(message3, 20);

   charDisplay_.setLine(4);
   charDisplay_.writeMessage(message4, 20);
}

void Dashboard::displayTest()
{
    char message1[] = "ACTL Volt:  XXX.X  V";
    char message2[] = "ACTL Amp:   XXX.X  A";
    char message3[] = "Set  Amp:   XXX.X  A";
    char message4[] = "Set Speed:  XXX km/h";

    charDisplay_.setLine(1);
    charDisplay_.writeMessage(message1,20);

    charDisplay_.setLine(2);
    charDisplay_.writeMessage(message2, 20);

    charDisplay_.setLine(3);
    charDisplay_.writeMessage(message3, 20);

    charDisplay_.setLine(4);
    charDisplay_.writeMessage(message4, 20);
}
