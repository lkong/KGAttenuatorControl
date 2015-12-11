/*
 
	Created by WilliamK @ Wusik Dot Com (c) 2010
	http://arduino.wusik.com

*/

#ifndef ENCODER_h
#define ENCODER_h

#include <inttypes.h>
#include "HardwareSerial.h"

const int8_t encodeSeq[] = {1, 3, 0, 2};
#define clickDebounceTime 50
#define clickHoldTime 500

// ------------------------------------------------------------------------------------------- //
class Encoder
{
public:
	Encoder(int8_t pinLeft, int8_t pinRight, int8_t pinClick);
	bool tick(void);
	void lowLevelTick(void);
	void lowLevelClick(void);
	float getPosition(void);
	void setMinMax(float _min, float _max);
	void setPosition(float _position);
	void setRate(float _rate);
	bool hasChanged(void);
	bool hasClick(void);
	bool onClickHold(void);
	void setIntegerMode(bool mode);

private:
	int8_t encoderPinLeft;
	int8_t encoderPinRight;
	int8_t encoderPinClick;

	float prevPosition;
	float maxValue;
	float minValue;
	float currentPosition;
	float moveRate;
	int8_t tempPosition;
	int8_t tempPosition2;
	bool isForward;
	bool isClickedEvent;
	bool isClicked;
	int8_t prevClickValue;
	int8_t newClickValue;
	bool integerMode;
	unsigned long timeLastClick;
	unsigned long timeClickStarted;
};

#endif
