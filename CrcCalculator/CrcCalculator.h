#ifndef CrcCalculator_H
#define CrcCalculator_H

/*
* CRC calculator:
* source code from: https://developer.mbed.org/users/EmLa/code/CRC16/
* Table borrowed from wsutil/crc16.h by Richard van der Hoff (no idea on link, sorry)
* Refer to this for more information http://www.zlib.net/crc_v3.txt
* This is designed to match the QT qChecksum calculation with the parameters of:
*   Name   : "CRC-16"
*   Width  : 16
*   Poly   : 1021
*   Init   : FFFF
*   RefIn  : True
*   RefOut : True
*   XorOut : FFFF
*   Check  : 906E
*/
class CrcCalculator
{
public:
   CrcCalculator();
   ~CrcCalculator();

   static unsigned short calculateCrc16(unsigned char* input, int lenght);

private:
   static const unsigned short crc16table[256];
};

#endif
