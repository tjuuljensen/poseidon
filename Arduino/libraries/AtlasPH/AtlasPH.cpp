/*
AtlasPHCircuit Library AtlasPH.cpp
Author: Torsten Juul-Jensen (2013)

Used for controlling pH circuit from Atlas Scientific without using delay()
	
*/

#include "AtlasPH.h"
#include "SoftwareSerial.h"

AtlasPH::AtlasPH(uint8_t rx, uint8_t tx){

	//Variables for each pH probe connected to system
	unsigned int uiPHTimerStep; // Number of milliseconds from now until a certain target state is accomplished for a LED
	unsigned long uloPHTimerPrevious; //Last registered time - used for controlling LED timer actions
	unsigned long uloPHTimerCurrent; //Current time - used for controlling LED timer actions
	byte bytPHAutoRead=1;               //If this one is set, the arduino autoreads the pH value each intPHAutoReadInterval
	int intPHAutoReadInterval=5;	//Default autoread Interval in *seconds*
	char chaPHbuffer[20];                  //we make a 20 byte character array to hold incoming data from the pH. 
	byte bytInitDone=0;                    //Controls that the Arduino has been initialized before reading values (specifically when autoread is on)
	float floPHvalue=0;                        //used to hold a floating point number that is the pH. 
	byte bytPHInputReceived=0;            //used to identify when we have received a string from the pH circuit.

	//Init pH circuit serial 
	SoftwareSerial phCircuit(RX, TX); //Initialize serial connection to pH board
	
	//GetVersion
	//String strVersion=getVersion()
}

/*
Functions:

bool autoread 
bool SetAutoreadInterval
float readPH
led
void s_cal()              //calibrate to a pH of 7
void f_cal()              //calibrate to a pH of 4 
void t_cal()              //calibrate to a pH of 10.00
void factoryReset()    //factory defaults the pH circuit
void continuous()    //Puts the pH circuit into continuous mode
void getInfo()           //get device info
void getVersion
void setBaudRate(unsigned long)
*/

void init() {
}

void readph() {
	//read pH from atlas device
	//check hw version
}

 
void phAutoInit() {
	if(bytInitDone==0){       //if the Arduino just booted up, we need to set some things up first.   
	  phCircuit.print("e\r");     //take the pH Circuit out of continues mode. 
	  delay(50);                  //on start up sometimes the first command is missed. 
	  phCircuit.print("e\r");     //so, let’s send it twice.
	  delay(50);                 //a short delay after the pH Circuit was taken out of continues mode is used to make sure we don’t over load it with commands.
	  bytInitDone=1;                 //bytInitDone is completed, let's not do this again during normal operation. 
	}
}


void readPHCircuit() {
	
	byte bytPHInputCount=0;       //holds how many characters have been received.

    if(phCircuit.available() > 0){        //if the pH Circuit has sent a character.
		bytPHInputCount=phCircuit.readBytesUntil(13,chaPHbuffer,20); // read data sent from pH Circuit until <CR>. Count how many character have been received.  
		chaPHbuffer[bytPHInputCount]=0;  //Add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer. 
		bytPHInputReceived=1;                //a flag used when the Arduino is controlling the pH Circuit to let us know that a complete string has been received.
		}  
  
  if(bytPHAutoRead) { //pH auto read is enabled. Reads the pH value each 
  	uloPHTimerCurrent=millis(); //current time
	if(uloPHTimerCurrent-uloPHTimerPrevious>uiPHTimerStep) { //is it time to make a reading? Otherwise continue doing nothing
		uloPHTimerPrevious=uloPHTimerCurrent; //reset timer to now (save the last time we made a reading)
		phCircuit.print("R\r");             //make a single reading
	    if(bytPHInputReceived){             //did we get data back from the ph Circuit?
			floPHvalue=atof(chaPHbuffer);   //converts chaPHbuffer to a float for calculation and variable storing
			bytPHInputReceived=0; 		//reset the string received flag
		}             
	}		
  }  
}
 

void led(byte enabled) //turn the pH circuit LEDs on or off
{
  if(enabled)                //if enabled is > 0 
    phCircuit.print("L1\r");  //the LED's will turn ON 
  else                       //if enabled is 0        
    phCircuit.print("L0\r");  //the LED's will turn OFF
}


//pH circuit functions. Not enabled.

void s_cal(){               //calibrate to a pH of 7
  phCircuit.print("s\r");}   //send the "s" command to calibrate to a pH of 7.00


void f_cal(){               //calibrate to a pH of 4 
  phCircuit.print("f\r");}   //send the "f" command to calibrate to a pH of 4.00 


void t_cal(){               //calibrate to a pH of 10.00
  phCircuit.print("t\r");}   //send the "t" command to calibrate to a pH of 10.00  


void factoryReset(){    //factory defaults the pH circuit
  phCircuit.print("X\r");}   //send the "X" command to factory reset the device 

void continuous(){    //Puts the pH circuit into continuous mode
  phCircuit.print("C\r");}   //send the "C" command to factory reset the device 

void getInfo(){           //get device info
    phCircuit.print("I\r");} //send the "I" command to query the information - something like "P,V5.0,5/13"

void getVersion(){           //get device info
    phCircuit.print("I\r");
	strSerialInput.substring(3)
	} //send the "I" command to query the information - something like "P,V5.0,5/13"
