
//Clock pin
#define SPI_CLK 1
//DIN pin on Attenuator board
#define SPI_MOSI 2
//Device select pin, CS* on Attenuator board
#define DATTN_CS 0
#define ENCODER_LEFT 4
#define ENCODER_RIGHT 5
#define ENCODER_CLICK 6

#include <Wire.h>
#include "Font.h"
#include "DATTN.h"
#include "Encoder.h"

//  Attenuator controls
const byte attSetAddr = 0x38;
const byte attResetAddr = 0x3E;
const int offVolume = 100;
const int relayLatchTime = 3;    //  time in milliseconds
int newVol = offVolume ;

DATTN dattnSPI(SPI_MOSI, SPI_CLK, DATTN_CS);

Encoder myEncoder(ENCODER_LEFT,ENCODER_RIGHT,ENCODER_CLICK);
float newData = 0.0f;

void EncoderMoveInterrupt() { myEncoder.lowLevelTick(); Serial.println("tick"); }
void EncoderClickInterrupt() { myEncoder.lowLevelClick(); }

//==========================================================//
void setup()
{
  /*  for encoder  */
  Serial.begin(38400);

  Wire.begin();

  dattnSPI.InitDAttn();

  digitalWrite(DATTN_CS, HIGH);

  delay(10);

  for (int i = 1; i <= 8; ++i) {
    dattnSPI.SetRelay(i, false);
  }
  delay(2000);



//  attachInterrupt(2,EncoderMoveInterrupt,CHANGE);
//  attachInterrupt(3,EncoderMoveInterrupt,CHANGE);
//  attachInterrupt(4,EncoderClickInterrupt,CHANGE);

  myEncoder.setMinMax(1,100);
  myEncoder.setPosition(0);
  myEncoder.setRate(0.25f);
  myEncoder.setIntegerMode(true);

}

//==========================================================//
void loop()
{
  unsigned int v;
  String string1;

  //dattnSPI.SetVolume(0x1000);

//  if (myEncoder.hasChanged())
//  {
//    newData = myEncoder.getPosition();
//    Serial.print("> Position: ");
//    Serial.println(newData);
//
//    SetVolume((int)newData);
//
//  }
//  if (myEncoder.hasClick()) Serial.println("! Click !");
//  if (myEncoder.onClickHold()) { Serial.println(" ... Holding ..."); delay(500); }



  if (myEncoder.tick())
  {
    newData = myEncoder.getPosition();
    Serial.print("> Position: ");
    Serial.println(newData);
    if (myEncoder.hasClick()) Serial.println("! Click !");
    if (myEncoder.onClickHold()) { Serial.println(" ... Holding ..."); delay(500); }

    SetVolume((int)newData);
  }

}


void AttnSetup() {
  Wire.beginTransmission(attResetAddr);
  Wire.write(0);
  Wire.write(0xFF);
  Wire.endTransmission();
  delay(relayLatchTime);

  Wire.beginTransmission(attResetAddr+1);
  Wire.write(0);
  Wire.write(0xFF);
  Wire.endTransmission();
  delay(relayLatchTime);

  Wire.beginTransmission(attResetAddr);
  Wire.write(0);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(relayLatchTime);

  Wire.beginTransmission(attResetAddr+1);
  Wire.write(0);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(relayLatchTime);
}

void SetVolume(int newVol, byte addrOffset)
{
  int sendVal;

  //  now set
  sendVal = (byte)newVol ;
  Wire.begin();
  Wire.beginTransmission(attSetAddr+addrOffset);
  Wire.write(0);
  Wire.write(sendVal);
  Wire.endTransmission();

  delay(relayLatchTime) ;

  Wire.begin();
  Wire.beginTransmission(attSetAddr+addrOffset);
  Wire.write(0);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(relayLatchTime);


  // now reset
  sendVal = 0xFF ^ (byte)newVol;

  Wire.beginTransmission(attResetAddr+addrOffset);
  Wire.write(0);
  Wire.write(sendVal);
  Wire.endTransmission();

  delay(relayLatchTime);

  Wire.beginTransmission(attResetAddr+addrOffset);
  Wire.write(0);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(relayLatchTime);

}

void SetVolume(unsigned int v)
{
  unsigned char bb;
  bool bSet;

  //v = 128-v;

  bb = 0x01;

  for (int i = 8 ; i > 0; --i) {
    bSet = ((bb & v) != 0);

    dattnSPI.SetRelay(9-i, bSet);
    bb = bb << 1;
    //delay(1000);

  }
}




