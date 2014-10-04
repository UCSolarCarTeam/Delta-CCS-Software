/*-------------------------------------------------------
  Made for the dashboard OLED displays
  By Jordan Heinrichs on 2014-05-10 for the Solar Car Team
  Copyright (c) 2014 by University of Calgary Solar Car Team 
-------------------------------------------------------*/

#ifndef GRAPHICIMAGEDATA_H
#define GRAPHICIMAGEDATA_H

class GraphicImageData
{
public:

   static const unsigned char imageBoundsDigitLeftColumn[2];
   static const unsigned char imageBoundsDigitCenterColumn[2];
   static const unsigned char imageBoundsDigitRightColumn[2];  

   static const unsigned char imageBoundsDigitRow[2];

   static const unsigned char imageBoundsLeftSymbolColumn[2];
   static const unsigned char imageBoundsRightSymbolColumn[2];

   static const unsigned char imageBoundsTopSymbolRow[2];
   static const unsigned char imageBoundsBottomSymbolRow[2];

   static const unsigned int numberOfBytesInDigits;
   static const unsigned int numberOfBytesInSymbol;


   //These are 48 x 64 px
   static const unsigned char digit0[];
   static const unsigned char digit1[];
   static const unsigned char digit2[];
   static const unsigned char digit3[];
   static const unsigned char digit4[];
   static const unsigned char digit5[];
   static const unsigned char digit6[];
   static const unsigned char digit7[];
   static const unsigned char digit8[];
   static const unsigned char digit9[];  

   //These are 52 by 32 px
   static const unsigned char units[];
   static const unsigned char bluetooth[]; 
   static const unsigned char headlights[];
   static const unsigned char caution[];

};
#endif