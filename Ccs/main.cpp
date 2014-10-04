/*-------------------------------------------------------
  Made for the ccs mbed LPC-1768
  By Jordan Heinrichs on for the Solar Car Team
  Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

#include <mbed.h>

#include <Ccs.h>

int main()
{
   Ccs ccs;
   ccs.initalizeActions();

   while(1) 
   {
      //If there is no power benefits doing wating vs performing actions might 
      //as well just perform the action continusly.
      ccs.performActions();
      wait_ms(60);
   }  
}
