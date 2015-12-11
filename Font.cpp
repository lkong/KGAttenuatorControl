/*
  Font.cpp - Library to help manage font data for OLED Display
  Created by Kerry Gerontianos, March 2012
*/
#include <avr/pgmspace.h>

#include "Font.h"

#include "FontData.h"

unsigned char fontChar[128];

extern const unsigned char __fontSys8[] PROGMEM;
extern const unsigned char __fontBw24[] PROGMEM;

Font::Font(unsigned char *Face, unsigned char Width, unsigned char Height, unsigned char FirstChar, unsigned char LastChar)
{
  int i;
  
  for (i=0; i<32 && Face[i] != 0x00; ++i)
  {
    _face[i] = Face[i];
  }
  _face[i] = (unsigned char)0;
  
  _height = Height;
  _width = Width;
  _bytesPerChar = (int)_width * (int)_height / 8;
  
  _firstChar = FirstChar;
  _lastChar = LastChar;
  
  /*
  switch(Width) {
    case 24:
      _data = fontBw24;
      break;
    case 32:
      _data = fontBw32;
      break;
    default:
      _data = fontSys8;
  }
  */
}


unsigned char *Font::Face()
{
  return _face;
}


unsigned char Font::Height()
{
  return _height;
}


unsigned char Font::Width()
{
  return _width;
}


int Font::BytesPerChar()
{
  return _bytesPerChar;
}

unsigned char *Font::Data(unsigned char ch)
{
  unsigned char *res;

  if (_lastChar - _firstChar <= ch || true)  {  
    switch (_width) {
      case 24:
        for (int i = 0; i < _bytesPerChar; ++i)
          fontChar[i] = pgm_read_byte(__fontBw24+((ch-_firstChar)*_bytesPerChar)+i);
        
        res = fontChar;
        //res = __fontBw24[ch-_firstChar];
        break;
      default:
      {
        for (int i = 0; i < _bytesPerChar; ++i)
          fontChar[i] = pgm_read_byte(__fontSys8+((ch-_firstChar)*_bytesPerChar)+i);
        
        res = fontChar;
      }
    }
  }
  else
    res = 0;
  
  return res;
}


