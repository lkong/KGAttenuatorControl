/*
  OLED.h - Library to help manage font data for OLED Display
  Created by Kerry Gerontianos, March 2012
*/
  
#ifndef OLED_h
#define OLED_h

#include "Font.h"


//#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 0
#define WHITE 1


#define SH1106    0x01
#define SSD1306   0x02
#define SPI4      0x01
#define I2C       0x02


/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1306_128_64  128x64 pixel display

    SSD1306_128_32  128x32 pixel display

    You also need to set the LCDWIDTH and LCDHEIGHT defines to an 
    appropriate size

    -----------------------------------------------------------------------*/
#define SSD1306_128_64
//     #define SSD1306_128_32
/*=========================================================================*/

#if defined SSD1306_128_64 && defined SSD1306_128_32
  #error "Only one SSD1306 display can be specified at once in SSD1306.h"
#endif
#if !defined SSD1306_128_64 && !defined SSD1306_128_32
  #error "At least one SSD1306 display must be specified in SSD1306.h"
#endif

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define SH1106_X_PIXELS		128
#define SH1106_Y_PIXELS		64
#define SH1106_ROWS	        8



class OLED
{
  public:
    
    
    OLED(unsigned char Width, unsigned char Height, unsigned char ctrlType);
    OLED(unsigned char Width, unsigned char Height, unsigned char ctrlType, int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS);
    void InitOLED(void);
    void SendCommand(unsigned char com);
    void ClearDisplay(void);
    void SetXY(unsigned char row,unsigned char col);
    void SendChar(unsigned char data);
    void Write(unsigned char value);
    //void Write(unsigned char *string);
    void Write(unsigned char *data, int len);
    void Write(unsigned char row, unsigned char col, Font *font, unsigned char *string);
    void SendStr(unsigned char row, unsigned char col, Font *font, unsigned char *string);
    void SendStr(unsigned char row, unsigned char col, Font *font, String string);
    void SetDisplayOff ();
    void SetDisplayOn ();
    void SetHorizontalMode ();
    void SetVerticalMode ();
    void DisplayInvert();
    void DisplayNormal();
    void drawBitmap (unsigned char *bmpImage);
    void DeactivateScrolling();
    
  private:
    unsigned char _width;
    unsigned char _height;
    int8_t _sid;
    int8_t _sclk;
    int8_t _dc;
    int8_t _rst;
    int8_t _cs;
    unsigned char _ctrlType;
    bool _isSPI;
    bool _fastSpi;

    enum {
      OLED_I2C_Address    = 0x3c,
      SetLowColumnAddr    = 0x00,        //  0x00 - 0x0F or'd with command
      SetHighColumnAddr   = 0x10,        //  0x00 - 0x0F or'd with command
      ExternalVcc         = 0x01,
      SwitchCapVcc        = 0x02,
      SetMemoryMode       = 0x20,
      ScrollDeactivate    = 0x2E,
      SetStartLine        = 0x40,        //  0x00 - 0x3F or'd with command
      SetContrast         = 0x81,
      SetChargePump       = 0x8D,
      SetSegRemap         = 0xA0,        //  0x00 - 0x01 or'd with command - reverse L-R direction of display (Mirror)
      EntireDisplayOff    = 0xA4,
      EntireDisplayOn     = 0xA5,
      NormalDisplay       = 0xA6,
      InverseDisplay      = 0xA7,
      SetMultiplex        = 0xA8,
      
      DisplayOff          = 0xAE,
      DisplayOn           = 0xAF,
      SetPageStartAddr    = 0xB0,        //  0x00 - 0x07 or'd with command
      ComScanInc          = 0xC0,
      ComScanDec          = 0xC8,
      SetDisplayOffset    = 0xD3,
      SetDisplayClockDiv  = 0xD5,
      SetPreChargePeriod  = 0xD9,
      SetComPins          = 0xDA,
      SetVComHDeselectLvl = 0xDB,
    };
};

#endif
