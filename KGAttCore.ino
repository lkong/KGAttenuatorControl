
//Clock pin
#define SPI_CLK 1
//DIN pin on Attenuator board
#define SPI_MOSI 2
//Device select pin, CS* on Attenuator board
#define DATTN_CS 0

//NeoPixel ring pin
#define PIXEL_PIN 10

//NeoPixel number
#define PIXEL_COUNT 24

#define ENCODER_LEFT 4
#define ENCODER_RIGHT 5
#define ENCODER_CLICK 6

#include <Wire.h>
#include "Font.h"
#include "DATTN.h"
#include "Encoder.h"
#include "Adafruit_NeoPixel.h"

//  Attenuator controls
const byte attSetAddr = 0x38;
const byte attResetAddr = 0x3E;
const int offVolume = 100;
const int relayLatchTime = 3;    //  time in milliseconds
int newVol = offVolume ;

const int encoderMin=1;
const int encoderMax=100;

const int default_led_brightness = 100;

const int number_of_inputs = 4;
int current_input=0;

struct RGB{
    int R;
    int G;
    int B;

}
struct RGB inputRGBs[number_of_inputs];

DATTN dattnSPI(SPI_MOSI, SPI_CLK, DATTN_CS);


Encoder myEncoder(ENCODER_LEFT,ENCODER_RIGHT,ENCODER_CLICK);
float newData = 0.0f;

void EncoderMoveInterrupt() { myEncoder.lowLevelTick(); Serial.println("tick"); }
void EncoderClickInterrupt() { myEncoder.lowLevelClick(); }
Adafruit_NeoPixel ring=Adafruit_NeoPixel(PIXEL_COUNT,PIXEL_PIN, NEO_RGB + NEO_KHZ800);

void initInputRGBs()
{
    inputRGBs[0]={255,0,0};
    inputRGBs[1]={100,255,0};
    inputRGBs[2]={0,0,255};
    inputRGBs[3]={0,255,100};
}
//==========================================================//
void setup()
{
  // for NeoPixel ring
  NeoPixelRingSetup();
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

  myEncoder.setMinMax(encoderMin,encoderMax);
  myEncoder.setPosition(0);
  myEncoder.setRate(0.25f);
  myEncoder.setIntegerMode(true);

}

//==========================================================//
void loop()
{
  unsigned int v;
  String string1;

  if (myEncoder.tick())
  {
    newData = myEncoder.getPosition();
    Serial.print("> Position: ");
    Serial.println(newData);
    if (myEncoder.hasClick())
    {
        Serial.println("! Click !");
        NextNeoPixelRingCol();
    }
    if (myEncoder.onClickHold()) { Serial.println(" ... Holding ..."); delay(500); }

    SetVolume((int)newData);
    SetNeoPixelRingVol((int)newData);
  }

}

void NeoPixelRingSetup()
{
  initInputRGBs();
  ring.begin();
  ring.setBrigthness(default_led_brightness);
  ring.show();
}

void SetNeoPixelRingVol(int newVol)
{
    int number_of_leds_on={int)PIXEL_COUNT*newVol/encoderMax;
    for (int i=0;i<number_of_leds_on-1;i++)
    {
        ring.setPixelColor(i,initInputRGBs[current_input].R,initInputRGBs[current_input].G,initInputRGBs[current_input].B);
    }
    ring.setPixelColor(number_of_leds_on-1,255,255,255);
    ring.show();
}

void NextNeoPixelRingCol()
{
    current_input=(current_input+1)%number_of_inputs;
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




