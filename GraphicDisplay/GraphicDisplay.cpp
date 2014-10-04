/*-------------------------------------------------------
  Made for the dashboard OLED displays
  By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
  Copyright 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

#include "GraphicDisplay.h"

GraphicDisplay::GraphicDisplay(SPI *spi, PinName cs, PinName DataOrCommand, 
                               PinName resetSignal) : cs_(cs),
                               DataOrCommand_(DataOrCommand), 
                               resetSignal_(resetSignal)
{
  spi_ = spi;
  cs_ = 1;
  DataOrCommand_ = 1;
  resetSignal_ = 0;
  wait_ms(1);
  resetSignal_ =1;
}

void GraphicDisplay::init(void)
{
  spi_->format(8,3);
  spi_->frequency(FREQUENCY);

  Set_Command_Lock(0x12);               // Unlock Basic Commands (0x12/0x16) 
  Set_Display_On_Off(GRAPHIC_OFF);             // Display Off (0x00/0x01) 
  Set_Column_Address(0x1C, 0x5B);       

  Set_Row_Address(0x00, 0x3F);
  Set_Display_Clock(0x91);                // Set Clock as 80 Frames/Sec 
  Set_Multiplex_Ratio(0x3F);              // 1/64 Duty (0x0F~0x3F) 
  Set_Display_Offset(0x00);               // Shift Mapping RAM Counter (0x00~0x3F) 
  Set_Start_Line(0x00);                   // Set Mapping RAM Display Start Line (0x00~0x7F) 
  Set_Remap_Format(0x14);                 // Set Horizontal Address Increment 
                                          //     Column Address 0 Mapped to SEG0 
                                          //     Disable Nibble Remap 
                                          //     Scan from COM[N-1] to COM0 
                                          //     Disable COM Split Odd Even 
                                          //     Enable Dual COM Line Mode 
  Set_GPIO(0x00);                         // Disable GPIO Pins Input 
  Set_Function_Selection(0x01);           // Enable Internal VDD Regulator 
  Set_Display_Enhancement_A(0xA0,0xFD);   // Enable External VSL  
  Set_Contrast_Current(0x9F);             // Set Segment Output Current 
  Set_Master_Current(0x0F);               // Set Scale Factor of Segment Output Current Control 
  //Set_Gray_Scale_Table();               // Set Pulse Width for Gray Scale Table 
  Set_Linear_Gray_Scale_Table();          //set default linear gray scale table 
  Set_Phase_Length(0xE2);                 // Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks 
  Set_Display_Enhancement_B();        // Enhance Driving Scheme Capability (0x00/0x20) 
  Set_Precharge_Voltage(0x3E);            // Set Pre-Charge Voltage Level as 0.60*VCC 
  Set_Precharge_Period(0x08);             // Set Second Pre-Charge Period as 8 Clocks 
  Set_VCOMH(0x07);                        // Set Common Pins Deselect Voltage Level as 0.86*VCC 
  Set_Display_Mode(GRAPHIC_MODE_NORMAL);  // Normal Display Mode (0x00/0x01/0x02/0x03) 
  Set_Partial_Display(0x01,0x00,0x00);    // Disable Partial Display 
  Set_Display_On_Off(GRAPHIC_ON);
}

void GraphicDisplay::test(void)
{
  cs_ = 0;
  Set_Command_Lock(0x12);              // Unlock Basic Commands (0x12/0x16) 
  Set_Display_On_Off(0x00);           // Display Off (0x00/0x01) 
  cs_ = 1;
}

//0x12 for unlock (reset) 0x15 for lock
  void GraphicDisplay::Set_Command_Lock(int input)
  {
    cs_ = 0;
    DataOrCommand_ = 0;
    spi_->write(0xFD);
    DataOrCommand_ = 1;
    spi_->write(input);
    cs_ = 1; 
  }
  
  //0x00 for off 0x01 for on
  void GraphicDisplay::Set_Display_On_Off(int input)
  {
    cs_ = 0;
    DataOrCommand_ = 0;
    if(input == GRAPHIC_OFF)
    {
      spi_->write(0xAE);
    }
    else if(input == GRAPHIC_ON)
    {
      spi_->write(0xAF);                //turn ON
    }

    cs_ = 1;  
  }
  
  void GraphicDisplay::GraphicDisplayOnOff(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write((0xA0 | input));
     cs_ = 1;
  }
  
  //0x1C and 0x5B not sure what this represents, I think limits the width of 
  //the picture. First one is start address, second one is end address
  void GraphicDisplay::Set_Column_Address(int input1, int input2)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0x15);
     DataOrCommand_ = 1;
     spi_->write(input1);
     spi_->write(input2);
     cs_ = 1;
  }
  
  //0x00 and 0x3F not sure what this represents, I think limits the width of 
  //the picture. First one is start address, second one is end address
  void GraphicDisplay::Set_Row_Address(int input1, int input2)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0x75);
     DataOrCommand_ = 1;
     spi_->write(input1);
     spi_->write(input2);
     cs_ = 1;
  }
  
  //Sets the FPS of the device. Higher uses more power, lower can flicker. 0x91
  //Sets the device as 80 frames per second.
  void GraphicDisplay::Set_Display_Clock(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xB3);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Sets the multiplex display ratio???? Unsure what it does but 1/64 Duty 
  // (0x0F~0x3F) is suggested.
  void GraphicDisplay::Set_Multiplex_Ratio(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xCA);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Sets the vertical shift. For our purposes we do not need it so it should be 
  // set as 0x00
  void GraphicDisplay::Set_Display_Offset(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xA2);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Chooses the start line, for now we want to start at 0x00
  void GraphicDisplay::GraphicDisplay::Set_Start_Line(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xA1);
     DataOrCommand_ = 1;
     spi_->write(input); 
     cs_ = 1;
  }
  
  //Sets how the screen refreshes. The example uses 0x14 for I think A? and B 
  //will just be the default value
  //A[0] = 0; Horizontal Address Increment
//A[0] = 1; Vertical Address Increment
//A[1] = 0; Disable Column Address remap
//A[1] = 1; Enable Column Address remap
//A[2] = 0; Disable Nibble remap
//A[2] = 1; Enable Nibble remap
//A[4] = 0; Scan from COM0 to COM[N-1]
//A[4] = 1; Scan from COM[N-1] to COM0
//A[5] = 0; Disable COM split Odd/Even
//A[5] = 1; Enable COM split Odd/Even
//B[4] = 0; Disable Dual COM mode
//B[4] = 1; Enable Dual COM mode
//Note: A[5] must be 0 if B[4] is 1.
  void GraphicDisplay::Set_Remap_Format(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xA0);
     DataOrCommand_ = 1;
     spi_->write(input); 
     spi_->write(0x11);
     cs_ = 1;
  }
  
  //Set GPIO for the decoder. I have no idea what it does. Going with the 
 // suggested value of them being disabled 0x00
  void GraphicDisplay::Set_GPIO(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xB5);
     DataOrCommand_ = 1;
     spi_->write(input); 
     cs_ = 1;
  }
  
  //We want to enable the internal VDD regultor. if A[0] = 0 external is selected
  //Therefore we want to do 0x01
  void GraphicDisplay::Set_Function_Selection(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xAB);
     DataOrCommand_ = 1;
     spi_->write(input); 
     cs_ = 1;
  }
  
//(0xA0,0xF) will enable external VSL and enhance low GS display quality.
  //The reset values (0xA2, 0xB5) for internal VSL and "Normal". Note I am not 
  //sure how this affects the display or even what it does. More research required
  void GraphicDisplay::Set_Display_Enhancement_A(int input1, int input2)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xB4);
     DataOrCommand_ = 1;
     spi_->write(input1);
     spi_->write(input2); 
     cs_ = 1;
  }
  
  //Higher number more contrast. Defaults to 0x7F. 
  void GraphicDisplay::Set_Contrast_Current(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xC1);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Can reduce output for all colours. 0x0F is no change (default). 
  void GraphicDisplay::Set_Master_Current(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xC7);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Sets Default the pulse width of Gamma Setting 0. Refer to Section 8.8 for details
  void GraphicDisplay::Set_Linear_Gray_Scale_Table()
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xB9);
     cs_ = 1;
  }
  
  // Set Phase 1 as 5 Clocks & Phase 2 as 14 Clocks. Unsure of the reason of this.
  //Using the numbers given in the example code.
  void GraphicDisplay::Set_Phase_Length(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xB3);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //This triple byte command is sent to enhance the display performance.
//User is recommended to set A[5:4] to 00b. Doing what is suggested (their 
  //suggested inputs do not make sense so I am ignoring them
  void GraphicDisplay::Set_Display_Enhancement_B()
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xD1);
     DataOrCommand_ = 1;
     spi_->write(0x82);
     spi_->write(0x20); 
     cs_ = 1;
  }
  
  //Their value does not agree with what the datasheet says for 0.6*Vcc precharge
  //I am going with the value off the datasheet (0x3E)
  void GraphicDisplay::Set_Precharge_Voltage(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xBB);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Default is 0x08 which is what is suggested.
  void GraphicDisplay::Set_Precharge_Period(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xB6);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Sets the VCOMH voltage level
  void GraphicDisplay::Set_VCOMH(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xBE);
     DataOrCommand_ = 1;
     spi_->write(input);
     cs_ = 1;
  }
  
  //Sets display mode 0x00 is all off, 0x01 entire display on, 0x02 normal, 
  //0x03 inverse
  void GraphicDisplay::Set_Display_Mode(int input)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     if(input == GRAPHIC_MODE_OFF)
     {
        spi_->write(0xA4);
     }
     else if(input == GRAPHIC_MODE_ON)
     {
       spi_->write(0xA5);
     }
     else if(input == GRAPHIC_MODE_NORMAL)
     {
       spi_->write(0xA6);
     }
     else if(input == GRAPHIC_MODE_INVERTED)
     {
       spi_->write(0xA7);
     }
     cs_ = 1;
  }
  
  //Exits Partial Display Mode. Not sure what their inputs are.
  void GraphicDisplay::Set_Partial_Display(int input1, int input2, int input3)
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0xA9);
     cs_ = 1;
  }

  void GraphicDisplay::Write_To_RAM()
  {
     cs_ = 0;
     DataOrCommand_ = 0;
     spi_->write(0x5C);
     cs_ = 1;
  }

  void GraphicDisplay::fillRamWithInput(int input)
  {
    Set_Column_Address(0x1C, 0x5B);       

    Set_Row_Address(0x00, 0x3F);
      Write_To_RAM();
      cs_ = 0;
      DataOrCommand_ = 1;
      for(unsigned int i = 0; i<8192; i++)
      {
        spi_->write(input & 0xFF);
      }
      cs_ = 1;
  }

void GraphicDisplay::displayPicture(const unsigned char *data)
{
  Set_Column_Address(0x1C, 0x5B);       
  Set_Row_Address(0x00, 0x3F);
  Write_To_RAM();
  cs_ = 0;
  DataOrCommand_ = 1;
   
  for( int i = 0; i<8192; i++)  //8192=256/2*64 all the memory location
  {
    spi_->write(data[i]);  
    wait_us(1);
  }

  cs_ = 1;
}

void GraphicDisplay::displayData(const unsigned char *data,
                                 const unsigned char *columnBounds,
                                 const unsigned char *rowBounds, 
                                 const unsigned int numberOfBytes)
{
  Set_Column_Address(columnBounds[0], columnBounds[1]);
  Set_Row_Address(rowBounds[0], rowBounds[1]);
  Write_To_RAM();
  cs_ = 0;
  DataOrCommand_ = 1;

  for (int i = 0; i < numberOfBytes; ++i)
  {
    spi_->write(data[i]);
  }
}

void GraphicDisplay::fillRamSectionWithInput(int input,
                              const unsigned char *columnBounds,
                              const unsigned char *rowBounds, 
                              const unsigned int numberOfBytes)
{
  Set_Column_Address(columnBounds[0], columnBounds[1]);
  Set_Row_Address(rowBounds[0], rowBounds[1]);
  Write_To_RAM();
  cs_ = 0;
  DataOrCommand_ = 1;

  for (int i = 0; i < numberOfBytes; ++i)
  {
    spi_->write(input);
  }
}
