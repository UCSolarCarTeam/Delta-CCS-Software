#include <CharDisplay.h>
#include <GraphicDisplay.h>
#include <GraphicImageData.h>
#include <VehicleData.h>

#include <stdio.h>

#include <Dashboard.h>

Dashboard::Dashboard(PinName mosi, PinName miso, PinName clock, PinName chipSelectCharDisplay,
            PinName chipSelectGraphicDisplay, PinName graphicDisplayDataSignal,
            PinName graphicDisplayReset,
            VehicleData& vehicleData) 
            : serialCommunication_(mosi, miso, clock)
            , graphic_(&serialCommunication_, chipSelectGraphicDisplay,
                       graphicDisplayDataSignal, graphicDisplayReset)
            , charDisplay_(&serialCommunication_, chipSelectCharDisplay)
            , vehicleData_(vehicleData)
            {
               vehicleData_.pc.printf("init dashboard\n");
            }
            
void Dashboard::initAll()
{
  wait_ms(1); //wait for power to stabalize on the displays
  charDisplay_.init();
  graphic_.init();
  wait_ms(1); //Wait to make sure it is all completed.
  graphic_.fillRamWithInput(0x00);
  wait_ms(1);
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
 /*
   int vehicleVelocityKph = vehicleData_.vehicleVelocityKph;

   graphic_.displayData(findDigitNumber(vehicleVelocityKph / 100),
                        graphicImageData_.imageBoundsDigitLeftColumn,
                        graphicImageData_.imageBoundsDigitRow,
                        graphicImageData_.numberOfBytesInDigits);
   vehicleVelocityKph %= 100;
   graphic_.displayData(findDigitNumber(vehicleVelocityKph / 10),
                        graphicImageData_.imageBoundsDigitCenterColumn,                      
                        graphicImageData_.imageBoundsDigitRow,
                        graphicImageData_.numberOfBytesInDigits);
   vehicleVelocityKph %= 10;
   graphic_.displayData(findDigitNumber(vehicleVelocityKph),
                        graphicImageData_.imageBoundsDigitRightColumn,
                        graphicImageData_.imageBoundsDigitRow,
                        graphicImageData_.numberOfBytesInDigits);

   graphic_.displayData(graphicImageData_.units,
                        graphicImageData_.imageBoundsLeftSymbolColumn,
                        graphicImageData_.imageBoundsBottomSymbolRow,
                        graphicImageData_.numberOfBytesInSymbol);

   if (vehicleData_.bluetoothConnected)
   {
      graphic_.displayData(graphicImageData_.bluetooth,
         graphicImageData_.imageBoundsRightSymbolColumn,
         graphicImageData_.imageBoundsTopSymbolRow,
         graphicImageData_.numberOfBytesInSymbol);
   }
   else
   {
      graphic_.fillRamSectionWithInput(0x00,
         graphicImageData_.imageBoundsRightSymbolColumn,
         graphicImageData_.imageBoundsTopSymbolRow,
         graphicImageData_.numberOfBytesInSymbol);
   }

   if (vehicleData_.faultDetected)
   {
      graphic_.displayData(graphicImageData_.caution,
         graphicImageData_.imageBoundsRightSymbolColumn,
         graphicImageData_.imageBoundsBottomSymbolRow,
         graphicImageData_.numberOfBytesInSymbol);
   }
   else
   {
      graphic_.fillRamSectionWithInput(0x00,
         graphicImageData_.imageBoundsRightSymbolColumn,
         graphicImageData_.imageBoundsBottomSymbolRow,
         graphicImageData_.numberOfBytesInSymbol);
   }

   if (vehicleData_.headlightOn)
   {
      graphic_.displayData(graphicImageData_.headlights,
         graphicImageData_.imageBoundsLeftSymbolColumn,
         graphicImageData_.imageBoundsTopSymbolRow,
         graphicImageData_.numberOfBytesInSymbol);
   }
   else
   {
      graphic_.fillRamSectionWithInput(0x00,
         graphicImageData_.imageBoundsLeftSymbolColumn,
         graphicImageData_.imageBoundsTopSymbolRow,
         graphicImageData_.numberOfBytesInSymbol);
   }*/
}

const unsigned char* Dashboard::findDigitNumber(int number )
{
  switch (number)
  {
    case 0:
      return graphicImageData_.digit0;
    case 1:
      return graphicImageData_.digit1;
    case 2:
      return graphicImageData_.digit2;
    case 3:
      return graphicImageData_.digit3;
    case 4:
      return graphicImageData_.digit4;
    case 5:
      return graphicImageData_.digit5;
    case 6:
      return graphicImageData_.digit6;
    case 7:
      return graphicImageData_.digit7;
    case 8:
      return graphicImageData_.digit8;
    case 9:
      return graphicImageData_.digit9;
  default:
      return graphicImageData_.digit0;
  }
}

void Dashboard::displayTest()
{
    char message1[] = "ACTL Volt:  XXX.X  V";
    char message2[] = "ACTL Amp:   XXX.X  A";
    char message3[] = "Set  Amp:   XXX.X  A";
    char message4[] = "Set Speed:  XXX km/h";
  
    //graphic_.displayPicture(); This has been ommitted due to code size 
                                //limitations.
    graphic_.fillRamWithInput(0xFF);
    
    charDisplay_.setLine(1);
    charDisplay_.writeMessage(message1,20);

    charDisplay_.setLine(2);
    charDisplay_.writeMessage(message2, 20);
    
    charDisplay_.setLine(3);
    charDisplay_.writeMessage(message3, 20);
    
    charDisplay_.setLine(4);
    charDisplay_.writeMessage(message4, 20);
}