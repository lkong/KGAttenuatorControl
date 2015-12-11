/*
  OLED.cpp - Library to help manage font data for OLED Display
  Created by Kerry Gerontianos, March 2012
*/
  
#include "Arduino.h"
#include <Wire.h>
#include "OLED.h"


//unsigned char fill_OLED=0x55;
#define OLED_address  0x3c


OLED::OLED(unsigned char Width, unsigned char Height, unsigned char ctrlType)
{
  _width = Width;
  _height = Height;
  _ctrlType = ctrlType;
  _isSPI = false;
}

OLED::OLED(unsigned char Width, unsigned char Height, unsigned char ctrlType, int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS)
{
  _width = Width;
  _height = Height;
  _sid = SID;
  _sclk = SCLK;
  _dc = DC;
  _rst = RST;
  _cs = CS;
  _ctrlType = ctrlType;
  _isSPI = true;
  _fastSpi = false;
}

void OLED::InitOLED()
{
  if (!_isSPI) {
    SendCommand(0xae);		//display off
    //delay(50); 
    //----------------------------REVERSE comments----------------------------//
    //  sendcommand(0xa0);		//seg re-map 0->127(default)
    //  sendcommand(0xa1);		//seg re-map 127->0
    //  sendcommand(0xc8);
    //  delay(1000);
    //----------------------------REVERSE comments----------------------------//
  
    //  added by kpg 
    SendCommand(NormalDisplay);            //Set Normal Display (default)
    //delay(50); 
    SendCommand(ScrollDeactivate);  //deactive scrolling
    //delay(50); 
  
    
    SendCommand(DisplayOn);		//display on
    //delay(50); 
  
    SendCommand(SetMemoryMode);            //Set Memory Addressing Mode
    SendCommand(0x02);            //Set Memory Addressing Mode ab Page addressing mode(RESET)  
  
    SendCommand(NormalDisplay);            //Set Normal Display (default)
  }
  else {
    //SPI.begin();
    pinMode(_sid, OUTPUT);
    pinMode(_sclk, OUTPUT);
    pinMode(_dc, OUTPUT);
    pinMode(_rst, OUTPUT);
    pinMode(_cs, OUTPUT);
  
    digitalWrite(_rst, HIGH);
    // VDD (3.3V) goes high at start, lets just chill for a ms
    delay(1);
    // bring0xset low
    digitalWrite(_rst, LOW);
    // wait 10ms
    delay(10);
    // bring out of reset
    digitalWrite(_rst, HIGH);
    // turn on VCC (9V?)


//    if (_fastSpi)
//      setClockDivider(SPI_CLOCK_DIV2);
    
    // LCD init section:
	
    //uint8_t invertSetting = invert ? 0xA7 : 0xA6;
    //Vpp &= 0x03;

    // Must reset LCD first!
    //SH1106_PORT &= ~PIN_RESET;
    //delay(1);
    //SH1106_PORT |= PIN_RESET;


    SendCommand(DisplayOff);                 //display off
    
    SendCommand(SetMemoryMode);              //Set Memory Addressing Mode
    SendCommand(0x02);                       //Set Memory Addressing Mode ab Page addressing mode(RESET)  
    SendCommand(SetLowColumnAddr | 0x00);    //set lower column address
    SendCommand(SetHighColumnAddr | 0x00);   //set higher column address
    SendCommand(SetStartLine | 0x00);        //set display start line
    SendCommand(SetPageStartAddr | 0x00);    //set page address
    SendCommand(SetContrast);                //contract control
    SendCommand(0x8F);                       //128
    SendCommand(SetSegRemap | 0x00);         //set segment remap 0x01 and ComScanDec to flip display 
    SendCommand(ComScanInc);                 //Com scan direction
    SendCommand(NormalDisplay);              //normal / reverse
    SendCommand(SetMultiplex);               //multiplex ratio
    SendCommand(0x3F);                       //duty = 1/32
    SendCommand(SetChargePump);              //external VCC   
    SendCommand(0x14);    
    SendCommand(SetDisplayOffset);           //set display offset
    SendCommand(0x00);                       
    SendCommand(SetDisplayClockDiv);         //set osc division
    SendCommand(0x00);
    SendCommand(SetDisplayClockDiv);         //set osc division
    SendCommand(0x80);
    SendCommand(SetPreChargePeriod);         //set pre-charge period
    SendCommand(0xF1);                       //  was 0x1F
    SendCommand(SetComPins);                 //set COM pins
    SendCommand(0x12);
    SendCommand(SetVComHDeselectLvl);        //set vcomh
    SendCommand(0x40);
    
    SendCommand(DisplayOn);                  //display ON
  }
}

void OLED::SendCommand(unsigned char com)
{
  if (!_isSPI) {
    Wire.beginTransmission(OLED_address);     //begin transmitting
    Wire.write(0x80);                          //command mode
    Wire.write(com);
    Wire.endTransmission();                    // stop transmitting
  }
  else {
    digitalWrite(_cs, HIGH);
    digitalWrite(_dc, LOW);
    digitalWrite(_cs, LOW);
    //spiWrite(com);
    shiftOut(_sid, _sclk, MSBFIRST, com);
    digitalWrite(_cs, HIGH);
  }
}

void OLED::ClearDisplay(void)
{
  unsigned char i,k;
  for(k=0;k<8;k++)
  {	
    SetXY(k,0);    
    {
      for(i=0;i<128;i++)     //clear all COL
      {
        Write((unsigned char)0);         //clear all COL
        //delay(10);
      }
    }
  }
}

void OLED::SetXY(unsigned char row,unsigned char col)
{
  if (_ctrlType == SSD1306) {
    SendCommand(0xb0+row);                //set page address
    SendCommand(0x00+(8*col&0x0f));       //set low col address
    SendCommand(0x10+((8*col>>4)&0x0f));  //set high col address
  }
  else {    // SH1106
    SendCommand(0xb0+row);                //set page address
    SendCommand(0x00+((8*col+2)&0x0f));       //set low col address
    SendCommand(0x10+(((8*col>>4)+0)&0x0f));  //set high col address  
  }
}


void OLED::Write(unsigned char value)
{
  if (!_isSPI) {
    Wire.beginTransmission(OLED_address); // begin transmitting
    Wire.write(0x40);//data mode
    Wire.write(value);
    Wire.endTransmission();    // stop transmitting
  }
  else {
    digitalWrite(_cs, HIGH);
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    //spiwrite(c);
    shiftOut(_sid, _sclk, MSBFIRST, value);
    digitalWrite(_cs, HIGH);
  }
}


void OLED::Write(unsigned char *data, int len)
{
  if (!_isSPI) {
    Wire.beginTransmission(OLED_address); // begin transmitting
    Wire.write(0x40);//data mode
    Wire.write(data, len);
    Wire.endTransmission();    // stop transmitting
  }
  else {
    digitalWrite(_cs, HIGH);
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    //spiwrite(c);
    for (int i=0; i < len; ++i)
      shiftOut(_sid, _sclk, MSBFIRST, data[i]);
    digitalWrite(_cs, HIGH);
  }
}


void OLED::Write(unsigned char row, unsigned char col, Font *font, unsigned char *string)
{
  unsigned char i=0;
  unsigned char b;
  int r = row;
  int c = col;
  int bytesPerChar;
  int bytesPerRow;
  int numRows;
  unsigned char width;
  unsigned char height;
  
  bytesPerChar = font->BytesPerChar();
  width = font->Width();
  height = font->Height();
  bytesPerRow = width / 8;
  
  while(*string)
  {
    r = row;
    SetXY(r,c);    

    /*    
    for(i=0; i < bytesPerChar; i++)
    {
      if ((i % width) == 0)
        SetXY(r++,c);
        
      b = font->Data(*string)[i];
      Write(b);
      //delay(10);
    }
    */
    
    numRows = bytesPerChar / height;
    for(i=0; i<3/*numRows*/; ++i)
    {
      SetXY(row+i,c);
//      Write((unsigned char*)(font->Data(*string)[i*bytesPerRow]),bytesPerRow);
      Write((unsigned char*)(font->Data(*string)[i*3]),3);
    }
    
    *string++;
    //c += width/8;
  }
}


void OLED::SendStr(unsigned char row, unsigned char col, Font *font, unsigned char *string)
{
  unsigned char i=0;
  unsigned char b;
  int r = row;
  int c = col;
  int bytesPerChar;
  unsigned char width;
  
  bytesPerChar = font->BytesPerChar();
  width = font->Width();
  
  while(*string)
  {
    r = row;
    SetXY(r,c);    
    
    for(i=0; i < bytesPerChar; i++)
    {
      if ((i % width) == 0)
        SetXY(r++,c);
        
      b = font->Data(*string)[i];
      Write(b);
      //delay(10);
    }
    
    /*
    numRows = bytesPerChar / width;
    for(i=0; i<numRows; ++i)
    {
      SetXY(row+i,c);
      Write((unsigned char*)(font->Data(*string)+i*bytesPerRow),bytesPerRow);
    }
    */
    
    *string++;
    c += width/8;
  }
}


void OLED::SendStr(unsigned char row, unsigned char col, Font *font, String string)
{
  unsigned char i=0;
  unsigned char b;
  int r = row;
  int c = col;
  int bytesPerChar;
  unsigned char width;
  const char * str = string.c_str();
  
  bytesPerChar = font->BytesPerChar();
  width = font->Width();
  
  while(*str)
  {
    r = row;
    SetXY(r,c);    
    
    for(i=0; i < bytesPerChar; i++)
    {
      if ((i % width) == 0)
        SetXY(r++,c);
        
      b = font->Data(*str)[i];
      Write(b);
    }
    *str++;
    c += width/8;
  }
}


void OLED::SetDisplayOff ()
{
  SendCommand(0xae);		//display off
}

void OLED::SetDisplayOn ()
{
  SendCommand(0xaf);		//display on
  //delay(50);
}

void OLED::SetHorizontalMode ()
{
  SendCommand(0x20);            //Set Memory Addressing Mode
  SendCommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
}

void OLED::SetVerticalMode ()
{
  SendCommand(0x20);            //Set Memory Addressing Mode
  SendCommand(0x01);            //Set Memory Addressing Mode ab Vertical addressing mode
}


void OLED::DisplayInvert()
{
  SendCommand(0xa7);    //Set Inverse Display
}


void OLED::DisplayNormal()
{
  SendCommand(0xa6);    //Set normal Display
}


void OLED::drawBitmap (unsigned char *bmpImage)
{
  ClearDisplay();
  //delay(50);  

  SendCommand(0xa6);  //Set Normal Display

  SendCommand(0xae);		//display off
  SendCommand(0x20);            //Set Memory Addressing Mode
  SendCommand(0x0);            //Set Memory Addressing Mode ab Horizontal addressing mode

  SetXY(0,0);

  if (!_isSPI || true) {
    //==========================================================//
    for(int i=0;i<128*8;i++)     // show 128* 64 picture
    {
      Write(bmpImage[i]);
    }
    //==========================================================//
  }
  else {
    for(int k=0;k<8;k++)
    {	
      SetXY(k,0);    
      {
        for(int i=0;i<128;i++)     //clear all COL
        {
          Write(bmpImage[i*k]);         //clear all COL
          //delay(10);
        }
      }
    }
  }

  SendCommand(0xaf);            // display on
}

void OLED::DeactivateScrolling()
{
  SendCommand(0x2E);  //deactive scrolling
}

