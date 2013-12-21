/*
AtlasPHCircuit Library AtlasPH.cpp
Author: Torsten Juul-Jensen (2013)

Used for controlling pH circuit from Atlas Scientific without using delay()
	
*/

#include "AtlasPH.h"
#include "SoftwareSerial.h"

AtlasPH::AtlasPH(uint8_t rx, uint8_t tx){	
	SoftwareSerial phCircuit(rx, tx); //Initialize serial connection to pH board
	phCircuit.begin(38400);       //enable the software serial port (Atlas board)
	init()
}

AtlasPH::AtlasPH(uint8_t rx, uint8_t tx, unsigned long uloBaudRate){
	SoftwareSerial phCircuit(rx, tx); //Initialize serial connection to pH board
	//POINTER to uloBaudRate
	checkBaudRate(uloBaudRate) ? phCircuit.begin(uloBaudRate) : phCircuit.begin(38400);       //If the desired baud rate is valid, set it. Otherwise fallback to default 38.400
	init()
}

/*
Functions:
	bool autoread 
	void setBaudRate(unsigned long)
	getBuffer
	sendSerial
*/

void AtlasPH::init() {
	
	//Variables for each pH probe connected to system
	unsigned int uinTimerStep; // Number of milliseconds from now until a certain target state is accomplished for a LED
	unsigned long uloTimerPrevious; //Last registered time - used for controlling LED timer actions
	unsigned long uloTimerCurrent; //Current time - used for controlling LED timer actions
	char chaBuffer[20];                  //we make a 20 byte character array to hold incoming data from the pH. 
	float floPHvalue=7;                  //used to hold a floating point number that is the pH. Initialize at 7
	byte bytInputReceived=0;            //used to identify when we have received a string from the pH circuit.

	led(1);				//set pH circuit LED
	
	phCircuit.print("e\r");     //take the pH Circuit out of continues mode. 
	delay(50);                  //on start up sometimes the first command is missed. 
	phCircuit.print("e\r");     //so, let’s send it twice.
	delay(50);                 //a short delay after the pH Circuit was taken out of continues mode is used to make sure we don’t over load it with commands.
	byte bytContinuous=0;		//Holds the information if the circuit is in continuous mode
	
	float floVersion=getVersion();
	
	byte bytAutoread=1;               //If this one is set, the arduino autoreads the pH value each uinAutoreadInterval
	unsigned int uinAutoreadInterval=5;	//Default autoread Interval in *seconds*
	uinTimerStep=uinAutoreadInterval*1000; //Set timer step for autoreading pH
	uloTimerPrevious=millis();  //set initial timer value for autoreading pH
	
}

float AtlasPH::ph() {
	//read pH from atlas device
	//if inputreceived and (autoread OR continous) -> return last read value
	if (bytContinuous) {
	return floPHvalue;}
	else {
		phCircuit.print("R\r");             //make a single reading
		readCircuit()
	    if(bytInputReceived){             //did we get data back from the ph Circuit?
			return atof(chaBuffer);   //converts chaBuffer to a float for calculation and variable storing
			bytInputReceived=0; 	  //reset the string received flag
		}             
	}
}


bool checkBaudRate(unsigned long uloBaudRate) {
	
	unsigned long uloValidBaudRates[8]={300, 1200, 2400, 9600, 19200, 38400, 57600, 115200};
	for(int i=0; i<8; i++) {
		if(uloBaudRate==uloValidBaudRates[i]) {
			return true;
			break;
		}
		else {return false;}
	}
}

void AtlasPH::setAutoreadInterval(unsigned int uinInterval) {
	uinAutoreadInterval=uinInterval;
	uinTimerStep=uinAutoreadInterval*1000; //Set timer step for autoreading pH
}

void readCircuit() {
	
	bytInputReceived=0; 		//reset the string received flag
	byte bytInputCount=0;       //holds how many characters have been received.

    if(phCircuit.available() > 0){        //if the pH Circuit has sent a character.
		bytInputCount=phCircuit.readBytesUntil(13,chaBuffer,20); // read data sent from pH Circuit until <CR>. Count how many character have been received.  
		chaBuffer[bytInputCount]=0;  //Add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer. 
		bytInputReceived=1;                //a flag used when the Arduino is controlling the pH Circuit to let us know that a complete string has been received.
		}  
  
}
 
void AtlasPH::checkActions() {
  
  readCircuit()
  
  if(bytAutoread) { //pH autoread is enabled. Reads the pH value each 
  	uloTimerCurrent=millis(); //current time
	if(uloTimerCurrent-uloTimerPrevious>uinTimerStep) { //is it time to make a reading? Otherwise continue doing nothing
		uloTimerPrevious=uloTimerCurrent; //reset timer to now (save the last time we made a reading)
		phCircuit.print("R\r");             //make a single reading
		readCircuit()
	    if(bytInputReceived){             //did we get data back from the ph Circuit?
			floPHvalue=atof(chaBuffer);   //converts chaBuffer to a float for calculation and variable storing
			bytInputReceived=0; 		//reset the string received flag
		}             
	}		
  }  
 }

void AtlasPH::led(byte enabled) //turn the pH circuit LEDs on or off
{
  if(enabled)                //if enabled is > 0 
    phCircuit.print("L1\r");  //the LED's will turn ON 
  else                       //if enabled is 0        
    phCircuit.print("L0\r");  //the LED's will turn OFF
}


void AtlasPH::calibrate(uint8_t uinSolution) {
	switch(uinSolution) {
		case 4:
			phCircuit.print("f\r");   //send the "f" command to calibrate to a pH of 4.00
			break;
		case 7:
			phCircuit.print("s\r");   //send the "s" command to calibrate to a pH of 7.00
			break;
		case 10:
			phCircuit.print("t\r");   //send the "t" command to calibrate to a pH of 10.00
			break;
	}
	}

void AtlasPH::setTemperature() {
/*
Version 4.0
tt.tt (where “t” is temperature in °C) By transmitting a temperature to the pH Circuit a temperature 
compensated pH reading will be returned in the same manner as is seen when using the “R” command. 

Version 5.0
tt.tt (where “t” is temperature in °C) By transmitting a temperature to the pH Circuit a temperature 
compensated pH reading will be returned.
*/
}

void AtlasPH::factoryReset(){    //factory defaults the pH circuit
  phCircuit.print("X\r");}   //send the "X" command to factory reset the device 

void AtlasPH::continuous(){    //Puts the pH circuit into continuous mode
  phCircuit.print("C\r");}   //send the "C" command to factory reset the device 

String AtlasPH::info(){           //get device info
    phCircuit.print("I\r"); //send the "I" command to query the information - something like "P,V5.0,5/13"
	readCircuit() //read input from circuit
	String strSerialInput=chaBuffer;
	return strSerialInput;
	} 

float AtlasPH::getVersion(){           //get device info
    //if in continous mode or in autoread, disable this first
	phCircuit.print("I\r"); //send the "I" command to query the information - something like "P,V5.0,5/13"
	readCircuit() //read input from circuit
	String strSerialInput=chaBuffer;
	return atof(strSerialInput.substring(3,5));
	} 

	
/*
In version 5.0:
#nnnn Set 4 digit programmable ID number
 
Where nnnn is an ID number consisting of numbers 0-9 and letters A-Z. 
Setting the pH Circuit ID number is done by issuing the # command
followed by any combination of 4 ASCII letters or numbers.
The ID does not come set. A factory reset will NOT clear the ID number.

#! Reset ID number
Full proper syntax
#!<cr>
The pH Circuit will respond:
clr<cr>

#? Query ID number
Full proper syntax
#?<cr>
The pH Circuit will respond with the ID number that it has been given.
nnnn<cr>
If the ID number has not been set the pH Circuit will respond
No ID set<cr>
*/