/**
 *  Schulich Delta Central Communication System
 *  Made for the NHD-0420DZW-AB5
 *  Uses the SPI library on the Mbed LPC1768
 *  By Jordan Heinrichs for the University of Calgary Solar Car Team
 *
 *  Copyright (C) 2015 University of Calgary Solar Car Team
 *
 *  This file is part of the Schulich Delta Central Communication System
 *
 *  The Schulich Delta Central Communication System is free software: 
 *  you can redistribute it and/or modify it under the terms 
 *  of the GNU Affero General Public License as published by 
 *  the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  The Schulich Delta Central Communication System is distributed 
 *  in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 *  without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero 
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General 
 *  Public License along with the Schulich Delta Central Communication 
 *  System. If not, see <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#ifndef CHARDISPLAY_H
#define CHARDISPLAY_H

#include "mbed.h"

#define INIT_FUNCTION_SET 0x038
#define INIT_DISPLAY_OFF 0x008
#define INIT_DISPLAY_CLEAR 0x001
#define INIT_SET_DISPLAY_SHIFT 0x006
#define INIT_DISPLAY_HOME 0x002
#define INIT_DISPLAY_ON_CURSOR_ON 0x00F
#define INIT_DISPLAY_ON_CURSOR_OFF 0x00D
#define INIT_FREQUENCY_SET 3000000
#define INIT_MODE 3
#define INSTRUCTION_SET_NUM_BITS 10
#define DATA_SET_NUM_BITS 8
#define DATA_CONTROL_BITS 0x200

#define LINE_ONE_COMMAND 0x080
#define LINE_TWO_COMMAND 0x0C0
#define LINE_THREE_COMMAND 0x094
#define LINE_FOUR_COMMAND 0x0D4

enum LineNumbers {LINE_ONE =1 , LINE_TWO = 2, LINE_THREE = 3 , LINE_FOUR = 4};

class CharDisplay 
{
public:
  CharDisplay(SPI *spi,  PinName cs);
  
  //PROMISES: To initialize the OLED display. Must be done before anything 
  //            else is used.
  void init(bool cursorOn = false);

  //PROMISES: To write the specified length of the c-string starting at the last 
  //           cursor location. 
  //REQUIRES: Message need to be at least 1 character long 
  //          LengthOfMessage must be smaller than the length of string
  void writeMessage(char *messagePointer, int lengthOfMessage);
  
  //PROMISES: Sets the cursor to one of the lines. Can input 1 to 4 or LINE_ONE
  //           to LINE_FOUR
  void setLine(int lineNum);
  
  //PROMISES: Set the cursor position to the integer point specified. Use the 
  //            data sheet to find exact position.
  void setPosition(int position);
  
  //PROMISES: Clear the display. Does not reset the cursor location!
  void clear();

private:
  DigitalOut cs_;
  SPI *spi_;
};

#endif 