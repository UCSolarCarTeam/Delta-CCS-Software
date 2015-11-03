/**
 *  Schulich Delta Central Communication System
 *  Made for the Mbed LPC-1768
 *  By Jordan Heinrichs for the University of Calgary Solar Car Team
 *
 *  Copyright (C) 2015 University of Calgary Solar Car Team
 *
 *  This file is part of the Schulich Delta Central Communication System
 *
 *  The Schulich Delta Central Communication System is free software: 
 *  you can redistribute it and/or modify it under the terms 
 *  of the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     General Public License as published by 
 *  the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  The Schulich Delta Central Communication System is distributed 
 *  in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 *  without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,                                                                                                                                                                                                                                                                                                                                                     General 
 *  Public License along with the Schulich Delta Central Communication 
 *  System. If not, see <http://www.gnu.org/licenses/>.
 *
 *  For further contact, email <software@calgarysolarcar.ca>
 */

#pragma once

#include <mbed.h>

#include <MpptData.h>
class VehicleData;
union CanMessageFormatter;

class MpptCan
{
public:
   MpptCan(
      const PinName& canMpptTd,
      const PinName& canMpptRd,
      VehicleData& vehicleData);

   void initInterface();
   void sendCanData();

private:
   void readCan();

   void queryForDilithiumData();
   void setDilithiumState(bool mpptsOn);
   void sendMpptMode();
   void tryAndDecodeHelianthus(const CANMessage& receivedMessage);

   void writeFloatArrayToCharArray(const float* input, char* output);
   void writeCharArrayToFloat(const unsigned char* input, float* output);
   void populateCanMessageFormatter(const unsigned char* input, CanMessageFormatter& output);
   bool getHelianthusMessageNumbers(const unsigned int id, int& mpptNumber, int& messageNumber);

   void readDilithiumMessage(int mpptNumber, const unsigned char* messageData);
   void readStatus(int mpptNumber, const unsigned char* messageData);
   void readInput(int mpptNumber, const unsigned char* messageData);
   void readOutput(int mpptNumber, const unsigned char* messageData);

   void setupMpptData();
   int returnMpptDataIndexForMpptNumber(MpptData::Type type, int mpptNumber);

private:
   CAN mpptCan_;
   VehicleData& vehicleData_;
   int dilithiumQueryCounter_;
};
