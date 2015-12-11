/*
  Font.h - Library to help manage font data for OLED Display
  Created by Kerry Gerontianos, March 2012
*/
  
#ifndef Font_h
#define Font_h

//========================
//  Define the fonts here
class Font
{
  public:
    Font(unsigned char *Face, unsigned char Width, unsigned char Height, unsigned char FirstChar, unsigned char LastChar);
    unsigned char *Face();
    unsigned char Height();
    unsigned char Width();
    int BytesPerChar();
    unsigned char *Data(unsigned char ch);

  private:
    unsigned char _face[32];
    unsigned char _height;	// in pixels
    unsigned char _width;	// in pixels
    int _bytesPerChar;		// for fontSys8 this number is 8
    unsigned char *_data[];
    unsigned char _firstChar;
    unsigned char _lastChar;
};

#endif


