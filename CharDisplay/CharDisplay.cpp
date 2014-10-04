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
