/*-------------------------------------------------------
  Made for the dashboard OLED displays
  By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
  Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

#pragma once

#include "mbed.h"

#define FREQUENCY 3000000              //Max is 10 MHz 

#define GRAPHIC_ON 0x01
#define GRAPHIC_OFF 0x00

#define GRAPHIC_MODE_OFF 0x00
#define GRAPHIC_MODE_ON 0x01
#define GRAPHIC_MODE_NORMAL 0x02
#define GRAPHIC_MODE_INVERTED 0x03

class GraphicDisplay
{
public:
  GraphicDisplay(SPI *spi, PinName cs, PinName dataOrCommand, PinName resetSignal);
  
  void init(void);
  void test(void);
  void graphicDisplayOnOff(int input);
  void set_Display_Mode(int input);
  void fillRamWithInput(int input);
  void fillRamSectionWithInput(int input, const unsigned char *columnBounds,
                               const unsigned char *rowBounds, 
                               const unsigned int numberOfBytes);
  void displayPicture(const unsigned char *data);
  void displayData(const unsigned char *data, const unsigned char *columnBounds,
                   const unsigned char *rowBounds, const unsigned int numberOfBytes);  
  
private:
  
  SPI *spi_;
  DigitalOut cs_;               //Actve low
  DigitalOut dataOrCommand_;    //Active low
  DigitalOut resetSignal_;      //Active low.

  void set_Command_Lock(int input);
  void set_Column_Address(int input1, int input2);
  void set_Row_Address(int input1, int input2);
  void set_Display_On_Off(int input);
  void set_Display_Clock(int input);
  void set_Multiplex_Ratio(int input);
  void set_Display_Offset(int input);
  void set_Start_Line(int input);
  void set_Remap_Format(int input);
  void set_GPIO(int input);
  void set_Function_Selection(int input);
  void set_Display_Enhancement_A(int input1, int input2);
  void set_Contrast_Current(int input);
  void set_Master_Current(int input);
  void set_Linear_Gray_Scale_Table();
  void set_Phase_Length(int input);
  void set_Display_Enhancement_B();
  void set_Precharge_Voltage(int input);
  void set_Precharge_Period(int input);
  void set_VCOMH(int input);
  void set_Partial_Display(int input1, int input2, int input3);
  void write_To_RAM();
  
  static const unsigned char OLEDtest[];
  
};
