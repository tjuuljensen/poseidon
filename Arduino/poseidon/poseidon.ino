/*
Poseidon Arduino firmware
2013
Author: Torsten Juul-Jensen 

For use with the Poseidon Tank Monitor
	

*/

#include <EEPROM.h>
#include <SoftwareSerial.h>     //Include SoftwareSerial library for use with pH circuit serial communication
#include <PWMLED.h>
//#include <AtlasPH.h>

#define RX 12                   //What pin is used for serial RX com with ph board. Connected to TX (!!!)on ph board 
#define TX 13                   //What pin is used for serial TX com with ph board. Connected to RX (!!!) on ph board
#define PHENABLED 1				//PH reading enable´d
#define SERIALCONTROL 1 		//used for debugging and controlling over serial connection.
#define SERIALBUFFERLENGTH 35

#define MAXEEPROMADDR E2END //Defines last EEPROM address

#define WHITEPIN 9
#define GREENPIN 6
#define REDPIN 5
#define BLUEPIN 3

#define WHITE 0
#define GREEN 1
#define RED 2 
#define BLUE 3

#define NUMBEROFLEDS 4

PWMLED led[NUMBEROFLEDS] = {PWMLED(WHITEPIN),PWMLED(GREENPIN),PWMLED(REDPIN),PWMLED(BLUEPIN)}; //Create new instances of the PWMLED class. Pass their connected pins


//Variables for each pH probe connected to system
	unsigned int uiPHTimerStep; // Number of milliseconds from now until a certain target state is accomplished for a LED
	unsigned long uloPHTimerPrevious; //Last registered time - used for controlling LED timer actions
	unsigned long uloPHTimerCurrent; //Current time - used for controlling LED timer actions
	byte bytPHecho=1;
	byte bytPHAutoRead=1;               //If this one is set, the arduino autoreads the pH value each intPHAutoReadInterval
	int intPHAutoReadInterval=5;	//Autoread Interval in *seconds*
	char chaPHbuffer[20];                  //we make a 20 byte character array to hold incoming data from the pH. 
	byte bytInitDone=0;                    //Controls that the Arduino has been initialized before reading values (specifically when autoread is on)
	float floPHvalue=0;                        //used to hold a floating point number that is the pH. 
	byte bytPHInputReceived=0;            //used to identify when we have received a string from the pH circuit.

//Variables for serial connection
	char chaSerialInputBuffer[SERIALBUFFERLENGTH];             //we make a 20 byte character array to hold incoming data from a pc/mac/other. 
	byte bytSerialInputCount=0;     //we need to know how many characters have been received.     
	const String strPHprefix=  "ph.";


//Init pH circuit serial 
	SoftwareSerial phCircuit(RX, TX); //Initialize serial connection to pH board


void setup() {

	setLEDTestData();
  	InitSerialControl();
	InitPHcircuit();
	
}

void loop() {

  for(int i=0;i<NUMBEROFLEDS;i++) led[i].checkTimerActions();
  if(PHENABLED) readPHCircuit();
  
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
	
    if(SERIALCONTROL==1) Serial.begin(9600);         //enable the hardware serial port (USB)
	if(SERIALCONTROL==1) bytPHecho=1; //echo serial output from ph circuit on serial communication (USB)
}

void InitPHcircuit() {
	phCircuit.begin(38400);       //enable the software serial port (Atlas board)
	phSetLEDs(1);				//set pH circuit LED 
	if(bytPHAutoRead) phAutoInit(); //Get the curcuit ready for interval readings
	uiPHTimerStep=intPHAutoReadInterval*1000; //Set timer step for autoreading pH
	uloPHTimerPrevious=millis();  //set initial timer value for autoreading pH
}

 void serialEvent(){ //this function will trigger automatically when the data is received from the serial monitor.
	 
 if(SERIALCONTROL==1) {
    bytSerialInputCount=Serial.readBytesUntil(13,chaSerialInputBuffer,SERIALBUFFERLENGTH); //read data sent from the serial monitor until a <CR>. Count how many characters have been received.      
    chaSerialInputBuffer[bytSerialInputCount]=0; //Add a 0 to the spot in the array just after the last character we received. This is to avoid transmitting incorrect data that may have been left in the buffer. 
	
	String strSerialInput=chaSerialInputBuffer;
	
	if (strSerialInput.startsWith(strPHprefix,0)) { //if string is prefixed with e.g. "ph."
		String strCommand=strSerialInput.substring(3); //take the command from character 3 to end of string
		phCircuit.print(strCommand);           //we transmit the data received from the serial monitor(pc/mac/other) through the soft serial port to the pH Circuit. 
		phCircuit.print('\r');                   //End signal to pH Circuit with a <CR>.  
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
		bytPHAutoRead=0;
		Serial.println(">Autoread pH is now off");
	}
	else if (strSerialInput== "autoph on") {
		bytPHAutoRead=1;
		phAutoInit();
		Serial.println(">Autoread pH is now on");
	}			
        else if (strSerialInput=="led") {
          for(int i=0;i<NUMBEROFLEDS;i++) Serial.println(led[i].getState());
          }
	/*else if (strSerialInput.startsWith("autotime ",0)) {
        String strValue=strSerialInput.substring(8);
        char buf[strValue.length()];
        strValue.toCharArray(buf,strValue.length()+1);
		unsigned int uiNewTimer=atof(buf);
		Serial.println(uiPHTimerStep);
		uiPHTimerStep=uiNewTimer*1000;
		Serial.println(">Autotime changed");
		Serial.println(uiPHTimerStep);
	}	*/
	}
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
		if(SERIALCONTROL==1) {if(bytPHecho==1) Serial.println(chaPHbuffer);}          //lets transmit that data received from the pH Circuit to the serial monitor.
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
 

void phSetLEDs(byte enabled) //turn the LEDs on or off
{
  if(enabled)                //if enabled is > 0 
    phCircuit.print("L1\r");  //the LED's will turn ON 
  else                       //if enabled is 0        
    phCircuit.print("L0\r");  //the LED's will turn OFF
}

