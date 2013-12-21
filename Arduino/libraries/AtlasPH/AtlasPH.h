/*
AtlasPHCircuit Library AtlasPH.h
Author: Torsten Juul-Jensen (2013)

Used for controlling pH circuit from Atlas Scientific without using delay()
	
*/
#ifndef AtlasPH_H
#define AtlasPH_H

#include "Arduino.h" 

class AtlasPH{
  public:
    AtlasPH(uint8_t rx, uint8_t tx);
	AtlasPH(uint8_t rx, uint8_t tx, unsigned long uloBaudRate);
    void init();
	float ph();
	void setAutoreadInterval(unsigned int uinInterval);
	void checkActions();
	void led(byte enabled);
	void calibrate(uint8_t uinSolution);
	void setTemperature();
	void factoryReset();
	void continuous();
	String info();
	float getVersion();
	
	//up(how much)
	//down
  private:
	bool checkBaudRate(unsigned long uloBaudRate);
	void readCircuit();
};


#endif
