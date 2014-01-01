/*
PWMLED Library PWMLED.h
Author: Torsten Juul-Jensen (2013)
Author: Torsten Juul-Jensen 

Used for controlling PWM LEDs without using delay()
	
*/
#ifndef PWMLED_H
#define PWMLED_H

#include "Arduino.h" 

class PWMLED{
  public:
    PWMLED(uint8_t ledPin);
    void on();
	void off();
	void setBrightness(byte brightness);	
	byte getState(); 
	bool timerActive();
	void setFadeTimer(byte targetBrightness, unsigned int duration);
	void checkTimerActions();
	//up(how much)
	//down
  private:
	uint8_t pin;
	byte ledState ; //Holds the last set brightness of the actual LED
	bool ledTimerActive; // Variable to control whether a timer action is active
	byte ledTargetBrightness; //Controls the "to be" value of the LED
	byte ledControlOperator; //Stores -1 or 1, used for controlling LED direction
	int ledTimerStep; // Number of milliseconds from now until a certain target state is accomplished for a LED
    unsigned long ledTimerPrevious; //Last registered time - used for controlling LED timer actions
    unsigned long ledTimerCurrent; //Current time - used for controlling LED timer actions
};

#endif
