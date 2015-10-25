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

#include "CharDisplay.h"

CharDisplay::CharDisplay(SPI *spi, PinName cs) :  cs_(cs)
{
    spi_ = spi;
    cs_ = 1;
}

void CharDisplay::writeMessage(char *messagePointer, int lengthOfMessage)
{
   cs_ = 0;
   int firstChar = DATA_CONTROL_BITS | messagePointer[0];
   spi_->format(INSTRUCTION_SET_NUM_BITS, INIT_MODE);
   spi_->write(firstChar);
   
   spi_->format(DATA_SET_NUM_BITS, INIT_MODE);
   for(int i = 1; i<lengthOfMessage; i++)
   {
     spi_->write(messagePointer[i]);
   }
   cs_ = 1;
}

void CharDisplay::setPosition(int position)
{
  spi_->format(INSTRUCTION_SET_NUM_BITS, INIT_MODE);
  cs_ = 0;
  
  spi_->write(((position & 0x07F) | 0x080));
  
  cs_ = 1;
  
}

void CharDisplay::setLine(int lineNum)
{
  spi_->format(INSTRUCTION_SET_NUM_BITS, INIT_MODE);
  cs_ = 0;
  
  if(lineNum == LINE_ONE)
  {
    spi_->write(LINE_ONE_COMMAND);
  }
  else if(lineNum == LINE_TWO)
  {
    spi_->write(LINE_TWO_COMMAND);
  }
  else if(lineNum == LINE_THREE)
  {
    spi_->write(LINE_THREE_COMMAND);
  }
  else if(lineNum == LINE_FOUR)
  {
    spi_->write(LINE_FOUR_COMMAND);
  }

  cs_=1;
}

void CharDisplay::clear()
{
  spi_->format(INSTRUCTION_SET_NUM_BITS, INIT_MODE);
  cs_ = 0;
  spi_->write(INIT_DISPLAY_CLEAR);
  cs_ = 1;
}

void CharDisplay::init(bool cursorOn)
{
    cs_ = 1;
    spi_->format(INSTRUCTION_SET_NUM_BITS, INIT_MODE);
    spi_->frequency(INIT_FREQUENCY_SET);
    
    cs_ = 0;
    spi_->write(INIT_FUNCTION_SET);
    wait_ms(1);
    spi_->write(INIT_DISPLAY_OFF);
    spi_->write(INIT_DISPLAY_CLEAR);
    spi_->write(INIT_SET_DISPLAY_SHIFT);
    spi_->write(INIT_DISPLAY_HOME);
    if(cursorOn)
    {
      spi_->write(INIT_DISPLAY_ON_CURSOR_ON);
    }
    else
    {
      spi_->write(INIT_DISPLAY_ON_CURSOR_OFF);
    }
    wait_ms(2);

    cs_ = 1; 
}
