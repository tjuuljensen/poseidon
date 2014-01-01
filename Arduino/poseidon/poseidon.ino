/*
Poseidon Arduino firmware
2013
Author: Torsten Juul-Jensen 

For use with the Poseidon Tank Monitor
	

*/

#include <EEPROM.h>
#include <SoftwareSerial.h>     //Include SoftwareSerial library for use with pH circuit serial communication
#include <PWMLED.h>
#include <PoseidonConfig.h>
#include <PHOTOCELL.h>
#include <AtlasPH.h>


#define PHENABLED 1				//PH reading enable´d
#define SERIALCONTROL 1 		//used for debugging and controlling over serial connection.
#define SERIALBUFFERLENGTH 35

#define MAXEEPROMADDR E2END //Defines last EEPROM address

PWMLED led[NUMBEROFLEDS] = {PWMLED(WHITEPIN),PWMLED(GREENPIN),PWMLED(REDPIN),PWMLED(BLUEPIN)}; //Create new instances of the PWMLED class. Pass their connected pins
AtlasPH phSensor;
PHOTOCELL lightSensor;

//Variables for serial connection
	char chaSerialInputBuffer[SERIALBUFFERLENGTH];             //we make a 20 byte character array to hold incoming data from a pc/mac/other. 
	byte bytSerialInputCount=0;     //we need to know how many characters have been received.     
	const String strPHprefix=  "ph.";
	byte bytPHecho=0;

void setup() {

	setLEDTestData();
  	InitSerialControl();
	//InitPHcircuit();
	phSensor.led(1);
}

void loop() {

  for(int i=0;i<NUMBEROFLEDS;i++) led[i].checkTimerActions();
  if(PHENABLED) phSensor.checkActions();
  int intLightReading=lightSensor.value(); //just put the value into a variable
  Serial.println(intLightReading);
  //delay(2000);
  
}

void setLEDTestData(){
  // Make white G=160, R=230, B=255)
  
  led[WHITE].setBrightness(255);
  led[GREEN].setBrightness(160);
  led[RED].setBrightness(235);
  led[BLUE].setBrightness(255);
  
  led[WHITE].setFadeTimer(1, 10);
  led[GREEN].setFadeTimer(1, 10) ;
  led[RED].setFadeTimer(1, 10) ;
  led[BLUE].setFadeTimer(1, 10);
}
  
  
void readtemp() {
	//read temperature input using onewire protocol
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

void InitSerialControl() {
	
    if(SERIALCONTROL==1) Serial.begin(57600);         //enable the hardware serial port (USB)
	if(SERIALCONTROL==1) bytPHecho=1; //echo serial output from ph circuit on serial communication (USB)
}

 void serialEvent(){ //this function will trigger automatically when the data is received from the serial monitor.
	 
 if(SERIALCONTROL==1) {
    bytSerialInputCount=Serial.readBytesUntil(13,chaSerialInputBuffer,SERIALBUFFERLENGTH); //read data sent from the serial monitor until a <CR>. Count how many characters have been received.      
    chaSerialInputBuffer[bytSerialInputCount]=0; //Add a 0 to the spot in the array just after the last character we received. This is to avoid transmitting incorrect data that may have been left in the buffer. 
	
	String strSerialInput=chaSerialInputBuffer;
	
	if (strSerialInput.startsWith(strPHprefix,0)) { //if string is prefixed with e.g. "ph."
		String strCommand=strSerialInput.substring(3); //take the command from character 3 to end of string
		phSensor.ph();           //we transmit the data received from the serial monitor(pc/mac/other) through the soft serial port to the pH Circuit. 
		//phCircuit.print('\r');                   //End signal to pH Circuit with a <CR>.  
	}
	else if(strSerialInput== "help") {
		Serial.println(">Valid commands are:");
		Serial.println(">  help");
		Serial.println(">  echo on|off");
		Serial.println(">  autoph on|off");
		Serial.println(">  ph. followed by command to pH circuit. Ex: ph.i");
		//Serial.println(">  autotime 1|2|5|10|60");
		//Serial.println(">  led ");
		//Serial.println(">  ledtm(0,255,255)");
		//Serial.println(">  ");		
	}
	else if (strSerialInput== "echo off") {
		bytPHecho=0;
		Serial.println(">Echo is now off");
	}
	else if (strSerialInput== "echo on") {
		bytPHecho=1;
		Serial.println(">Echo is now on");
	}
	else if (strSerialInput== "autoph off") {
		//bytPHAutoRead=0;
		Serial.println(">Autoread pH is now off");
	}
	else if (strSerialInput== "autoph on") {
		//bytPHAutoRead=1;
		//phAutoInit();
		Serial.println(">Autoread pH is now on");
	}			
        else if (strSerialInput=="led") {
          for(int i=0;i<NUMBEROFLEDS;i++) Serial.println(led[i].getState());
          }
	}
 }
 
 
