/*
PWMLED Library PWMLED.cpp
Author: Torsten Juul-Jensen (2013)

Used for controlling PWM LEDs without using delay()
	
*/

#include "PWMLED.h"

PWMLED::PWMLED(uint8_t ledPin){
	this->pin=ledPin; //Substitute the variable ledPin with the shorter "pin"
	pinMode(this->pin,OUTPUT); //Set the pin to OUTPUT mode ready to receive inputs.
	byte ledState = 0; //Holds the last set brightness of the actual LED
	bool ledTimerActive=false; // Variable to control whether a timer action is active
	byte ledTargetBrightness=0; //Controls the "to be" value of the LED
	byte ledControlOperator=1; //Stores -1 or 1, used for controlling LED direction
	int ledTimerStep=0; // Number of milliseconds from now until a certain target state is accomplished for a LED
    unsigned long ledTimerPrevious=0; //Last registered time - used for controlling LED timer actions
    unsigned long ledTimerCurrent=0; //Current time - used for controlling LED timer actions
}

void PWMLED::on(void){
	if(ledState==0) analogWrite(pin,1);
}

void PWMLED::off(void){
	analogWrite(pin,0);
}

byte PWMLED::getState(){return ledState;}

void PWMLED::setBrightness(byte brightness){
	//Sets the brightness of the LED and stores the LED state (the current brightness) in a variable
	analogWrite(pin,brightness);
	ledState = brightness;
}

bool PWMLED::timerActive(){
	return ledTimerActive;
}

void PWMLED::setFadeTimer(byte targetBrightness, unsigned int duration){
	//Sets the LED variables in the desired state for the LED timer process
	if (ledState<targetBrightness)  
		{ledControlOperator = 1;}
	else if (ledState>targetBrightness) 
		{ledControlOperator = -1;} //The current state is higher than requested brightness and the LED should be dimmed 
	else 
		{return;} //Requested target brightness is the same as current brightness
	
	unsigned long ledControlStep= max(ledState,targetBrightness)-min(ledState,targetBrightness);
	ledTimerStep = duration*1000/ledControlStep;
    ledTimerActive=true; 
	ledTargetBrightness=targetBrightness;
	ledTimerActive=true;	
	ledTimerPrevious=millis();

	return;

	//To be made: Save state in EEPROM
}

void PWMLED::checkTimerActions() {
	//set the LED brightness if ledTimerActive is true
	if (ledTimerActive) {
		ledTimerCurrent=millis();
		if(ledTimerCurrent-ledTimerPrevious>ledTimerStep) {
			int newLedState=ledState+ledControlOperator;
			setBrightness(newLedState);
			ledTimerPrevious=ledTimerCurrent;
			if (ledState==ledTargetBrightness) ledTimerActive=false; 
		}
	}
}


extern PWMLED DEBUG_LED(13);

