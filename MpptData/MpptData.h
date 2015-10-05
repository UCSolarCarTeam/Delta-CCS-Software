/*-------------------------------------------------------
   Made for the ccs mbed LPC-1768
   By Jordan Heinrichs for the Solar Car Team
   Copyright (c) 2014 by University of Calgary Solar Car Team
-------------------------------------------------------*/

#pragma once

struct MpptData
{
   MpptData();

   enum Type
   {
      Helianthus = 0,
      Dilithium = 1
   };

   enum Mode
   {
      Static = 0,
      Dynamic = 1,
      IvTrace = 2,
      Open = 3,
      Invalid = 4
   };

   Type type;
   Mode mode;

   float voltageIn;
   float voltageOut;
   float currentIn;
   float currentOut;

   static int mpptCount;
};