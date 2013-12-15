/*
Poseidon Arduino firmware
2013
Author: Torsten Juul-Jensen 

For use with the Poseidon Tank Monitor
	
PIN layout:
	01
	02
	03 PWM
	04
	05 PWM
	06 PWM
	07
	08
	09 PWM
	10 PWM
	11 PWM
	12
	13
	A1
	A2
	A3
	A4
	A5
*/

#include <EEPROM.h>

#define MAXEEPROMADDR E2END //Defines last EEPROM address
const byte GREENPIN=6;
const byte REDPIN=5;
const byte BLUEPIN=3;
const byte WHITEPIN=9;

const byte WHITE=0;
const byte GREEN=1;
const byte RED=2;
const byte BLUE=3;

//#define FADESPEED 20     // make this higher to slow down

// Set LED variables
//	int FadeSpeed = 20;
	const byte numberOfLeds = 4; //number of LEDS connected to system
	byte ledPin[numberOfLeds] = {WHITEPIN, GREENPIN, REDPIN, BLUEPIN}; //initialize array to hold number of LEDS connected to system
	byte ledState[numberOfLeds];
	bool ledTimerActive[numberOfLeds];
	byte ledTargetBrightness[numberOfLeds];
	int ledTimerStep[numberOfLeds];
	//unsigned long ledControlStep[numberOfLeds]; //Unsigned long cause it is going to be used in a calculation
	byte ledControlOperator[numberOfLeds]; //Stores -1 or 1, used for controlling LED direction
    unsigned long ledTimerPrevious[numberOfLeds]; //Set Time calculation variables
    unsigned long ledTimerCurrent[numberOfLeds];


void setup() {
  // Initialize device
  // Set OUTPUT on LED pins
  for (int myLed = 0; myLed < numberOfLeds; myLed++)  {
    pinMode(ledPin[myLed], OUTPUT);      
     ledTimerActive[myLed]=false;  //Initialize LED timerActive  
     setled(myLed, 0); //Turn off All LEDs
     ledTargetBrightness[myLed]=0;
     ledState[myLed]=0;
     ledTimerPrevious[myLed]=0;
     ledControlOperator[myLed]=0; 
   }
  
  //TEST
  //Start LED job
  // Make white G=160, R=230, B=255)
  
  setled(WHITE,255);
  setrgb(230,160,255);
  
  setledtimer(WHITE, 1, 10) ;
  setledtimer(GREEN, 0, 60) ;
  setledtimer(RED, 10, 60) ;
  setledtimer(BLUE, 70, 60);
}

void loop() {
  // Code to be run repeatedly

	//Loop through the different LEDs and set their brightness if ledTimerActive is true
	for (int myLed = 0; myLed < numberOfLeds; myLed++)  {
		if (ledTimerActive[myLed]) {
			ledTimerCurrent[myLed]=millis();
			if(ledTimerCurrent[myLed]-ledTimerPrevious[myLed]>ledTimerStep[myLed]) {
				int newLedState=ledState[myLed]+ledControlOperator[myLed];
				setled(myLed,newLedState);
				ledTimerPrevious[myLed]=ledTimerCurrent[myLed];
				if (ledState[myLed]==ledTargetBrightness[myLed]) ledTimerActive[myLed]=false;
			}
		}
	}
	
}


void setled(byte myLedId, byte brightness) {
	//Sets the brightness of the LED and stores the LED state (the current brightness) in a variable
	analogWrite(ledPin[myLedId], brightness);
	ledState[myLedId]= brightness;
	
	//To be made: Save state in EEPROM

}

bool setledtimer(byte myLedId, byte TargetBrightness, unsigned int duration) {
	//Sets the LED variables in the desired state for the LED timer process
	//CHECK for same value (2BMade)
	if (ledState[myLedId]<TargetBrightness)  
		{ledControlOperator[myLedId] = 1;}
	else if (ledState[myLedId]>TargetBrightness) 
		{ledControlOperator[myLedId] = -1;} //The current state is higher than requested brightness and the LED should be dimmed 
	else 
		{return false;} //Requested target brightness is the same as current brightness
	
	unsigned long ledControlStep= max(ledState[myLedId],TargetBrightness)-min(ledState[myLedId],TargetBrightness);
	ledTimerStep[myLedId] = duration*1000/ledControlStep;
    ledTimerActive[myLedId]=true; 
	ledTargetBrightness[myLedId]=TargetBrightness;
	ledTimerActive[myLedId]=true;	
	ledTimerPrevious[myLedId]=millis();

	return true;

	//To be made: Save state in EEPROM
}


void setrgb(byte r, byte g, byte b) {
	//Sets input on RGB led if connected
	
	setled(RED, r);
	setled(GREEN, g);
	setled(BLUE, b);

}
	
void readtemp() {
	//read temperature input using onewire protocol
}

void readph() {
	//read pH from atlas device
	//check hw version
}

void readlight() {
}

void readflow() {
}

void checkalarm() {
}

void setdata() {
}

void readoutlet() {
}

void dimstatus() {
	//Check for DIM status - holds the current state of the dimming process 
	//Turnup/Dim down
	//How far
	//Next Action
}
void setdimactive() {
}

void checktrigger() {
}

void printserial() {
}

void cleareeprom() {
  //Arduino chips has different sizes of memory. This clears ALL memory space
  //write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i <= MAXEEPROMADDR; i++)
    EEPROM.write(i, 0);
    
  // TEST: turn the LED on when done clearing EEPROM
  // digitalWrite(13, HIGH);
  
}

void loadeepromsettings() {
	//requires eeprom.h loaded
}
