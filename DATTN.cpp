#include "Arduino.h"
#include <Wire.h>
#include "DATTN.h"

DATTN::DATTN(int8_t SID, int8_t SCLK, int8_t CS)
{
  _sid = SID;     // MOSI
  _sclk = SCLK;   // SCK
  _cs = CS;       // SS
  _isSPI = true;
  _fastSpi = false;
}

void DATTN::InitDAttn(void)
{
  if (!_isSPI) {
    ;
  }
  else {
    pinMode(_sid, OUTPUT);
    pinMode(_sclk, OUTPUT);
    pinMode(_cs, OUTPUT);
  
    digitalWrite(_cs, HIGH);
  }
}


void DATTN::SendCommand(unsigned char com)
{
  if (!_isSPI) {
    //    Wire.beginTransmission(OLED_address);     //begin transmitting
    //    Wire.write(0x80);                          //command mode
    //    Wire.write(com);
    //    Wire.endTransmission();                    // stop transmitting
    ;
  }
  else {
    pinMode(_cs, OUTPUT);

    digitalWrite(_cs, HIGH);
    digitalWrite(_cs, LOW);
    shiftOut(_sid, _sclk, MSBFIRST, com);
    
    digitalWrite(_cs, HIGH);
  }
}

void DATTN::SetVolume(unsigned int v)
{
  unsigned char bb;
  
  bb = 0x01;
  for (int i = 8 ; i > 0; --i) {
    SetRelay(i, bb & v);
    bb = bb << 1;
  }
//    digitalWrite(_cs, HIGH);
//    digitalWrite(_cs, LOW);
//    delay(3);
//    shiftOut(_sid, _sclk, MSBFIRST, (v >> 8));
//    shiftOut(_sid, _sclk, MSBFIRST, v);
//    shiftOut(_sid, _sclk, MSBFIRST, (v >> 8));
//    shiftOut(_sid, _sclk, MSBFIRST, v);
//    delay(3);
//    digitalWrite(_cs, HIGH);
//    delay(3);
//    
//    digitalWrite(_cs, LOW);
//    delay(3);
//    shiftOut(_sid, _sclk, MSBFIRST, 0);
//    shiftOut(_sid, _sclk, MSBFIRST, 0);
//    shiftOut(_sid, _sclk, MSBFIRST, 0);
//    shiftOut(_sid, _sclk, MSBFIRST, 0);
//    delay(3);
//    digitalWrite(_cs, HIGH);
//    delay(3);
  
}

unsigned int DATTN::SetRelay(unsigned char relayNo, bool set)
{
  unsigned int v;
  unsigned int shift;

  if (relayNo < 5) {
    relayNo = 5 - relayNo;    //  reverse the first four relays 4=1, 3=2, etc...
    shift = relayNo - 1;

    if (set) {
      shift = 7 - shift;
    }
  }
  else {
    shift = relayNo + 3;

    if (!set) {    // This is opposit from the first 4
      shift = 23 - shift;
    }
  }

  //  clip it at 8
  if (relayNo <= 8) {
    v = (1 << shift);
  }
  else {
    v = 0x08;
  }
  
  digitalWrite(_cs, HIGH);
  digitalWrite(_cs, LOW);
  delay(3);
  shiftOut(_sid, _sclk, MSBFIRST, (v >> 8));
  shiftOut(_sid, _sclk, MSBFIRST, v);
  shiftOut(_sid, _sclk, MSBFIRST, (v >> 8));
  shiftOut(_sid, _sclk, MSBFIRST, v);
  delay(3);
  digitalWrite(_cs, HIGH);
  delay(3);
  
  digitalWrite(_cs, LOW);
  delay(3);
  shiftOut(_sid, _sclk, MSBFIRST, 0);
  shiftOut(_sid, _sclk, MSBFIRST, 0);
  shiftOut(_sid, _sclk, MSBFIRST, 0);
  shiftOut(_sid, _sclk, MSBFIRST, 0);
  delay(3);
  digitalWrite(_cs, HIGH);
  delay(3);

  return shift;
}

