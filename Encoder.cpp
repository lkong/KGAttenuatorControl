/*
 
	Created by WilliamK @ Wusik Dot Com (c) 2010
	http://arduino.wusik.com
 
*/

#include "Arduino.h"
//#include "WConstants.h"
#include "Encoder.h"

// ------------------------------------------------------------------------------------------- //
Encoder::Encoder(int8_t pinLeft, int8_t pinRight, int8_t pinClick)
{ 
	encoderPinLeft = pinLeft;
	encoderPinRight = pinRight;
	encoderPinClick = pinClick;

	pinMode(encoderPinLeft, INPUT);
	digitalWrite(encoderPinLeft, HIGH);
	pinMode(encoderPinRight, INPUT);
	digitalWrite(encoderPinRight, HIGH);
	pinMode(encoderPinClick, INPUT);
	digitalWrite(encoderPinClick, HIGH);

	isClicked = false;
	prevClickValue = 1;
	integerMode = false;
	isClickedEvent = false;
	currentPosition = 0;
	prevPosition = 0;
	tempPosition = 0; 
	tempPosition2 = 0; 
	maxValue = 127; 
	minValue = 0;
	moveRate = 1.0f;
	timeLastClick = millis();
	timeClickStarted = 0;
}

// ------------------------------------------------------------------------------------------- //
void Encoder::lowLevelTick(void)
{
    tempPosition2 = (digitalRead(encoderPinRight) * 2) + digitalRead(encoderPinLeft);
    if (encodeSeq[tempPosition2] == tempPosition) currentPosition -= moveRate;
	    else if (encodeSeq[tempPosition] == tempPosition2) currentPosition += moveRate;
    tempPosition = tempPosition2;
}

// ------------------------------------------------------------------------------------------- //
void Encoder::lowLevelClick(void)
{
	newClickValue = digitalRead(encoderPinClick);
	if (newClickValue != prevClickValue)
	{
		if ((millis() - timeLastClick) < clickDebounceTime) return;

		if (newClickValue == 0 && !isClicked) 
		{
			isClickedEvent = true;
			isClicked = true;
			timeClickStarted = millis();
		}
		else if (newClickValue == 1) isClicked = false; 
		timeLastClick = millis();
	}
	prevClickValue = newClickValue;
}

// ------------------------------------------------------------------------------------------- //
bool Encoder::onClickHold(void)
{
	if (isClicked && (millis() - timeClickStarted) >= clickHoldTime) return true;
	return false;
}

// ------------------------------------------------------------------------------------------- //
bool Encoder::hasClick(void)
{
	if (isClickedEvent)
	{
		isClickedEvent = false;
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------- //
void Encoder::setIntegerMode(bool mode)
{
	integerMode = mode;
}

// ------------------------------------------------------------------------------------------- //
bool Encoder::tick(void)
{ 
	lowLevelTick();
	lowLevelClick();
	
	return (hasChanged() || isClickedEvent);
}

// ------------------------------------------------------------------------------------------- //
bool Encoder::hasChanged()
{
	currentPosition = constrain(currentPosition, minValue, maxValue);

	if (integerMode)
	{
		if ((int)getPosition() != (int)prevPosition)
		{
			prevPosition = getPosition();
			return true;
		}
	}
	else
	{
		if (getPosition() != prevPosition)
		{
			prevPosition = getPosition();
			return true;
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------- //
float Encoder::getPosition(void)
{
	if (integerMode) return (int)currentPosition;
	return currentPosition;
}

// ------------------------------------------------------------------------------------------- //
void Encoder::setMinMax(float _min, float _max) 
{ 
	minValue = _min;
	maxValue = _max;

	currentPosition = constrain(currentPosition, minValue, maxValue);
}

// ------------------------------------------------------------------------------------------- //
void Encoder::setPosition(float _position)
{
	currentPosition = _position;

	currentPosition = constrain(currentPosition, minValue, maxValue);
}

// ------------------------------------------------------------------------------------------- //
void Encoder::setRate(float _rate)
{
	moveRate = _rate;
}
