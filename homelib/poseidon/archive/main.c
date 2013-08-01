#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getmac_inc.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <curl/curl.h>
#include <my_global.h>
#include <mysql.h>
//global vars
unsigned char mac_address[6];
uint8_t outlet[] = {0,0,0,0,0,0,0,0}; // outlet status

uint8_t sensorActive[] = {0,0,0,0,0,0,0,0};
float  maxValues[] = {0,0,0,0,0,0,0,0};
float  minValues[] = {0,0,0,0,0,0,0,0};
// GPIO pin assignment
// RXD & TXD are asigned to the pH probe board TX & RX

//int serialTXDpin = 15; //GPIO 14;
//int serialRXDpin = 16; //GPIO 15;
int lightPin = 0;     // GPIO pin 17;
int dalasOneWire = 7; // GPIO pin 4 

int outletDataPin  = 1; // GPIO pin 18
int outletClockPin = 4; // GPIO pin 23
int outletLatchPin = 5; // GPIO pin 24

int fd ;  // file descriptor for the temperaturesensors


MYSQL mysql;

void lightReading(void)
{
	char strSQL[250];

	int reading = 0;
	printf("Light: ");
	pinMode(lightPin, OUTPUT);
	digitalWrite(lightPin,0);
	delay(100);
	pinMode(lightPin, INPUT);
	while(digitalRead(lightPin) == 0)
	{
		reading++;
	}
	pinMode(lightPin, OUTPUT);
	digitalWrite(lightPin,0);
	
	mysql_query(&mysql,"SELECT * FROM sensor WHERE id = 1");
	MYSQL_RES *result = mysql_store_result(&mysql);
	char strCalc[20];
	if(result){
		MYSQL_ROW row = mysql_fetch_row(result);
//		printf("Godt resultat %d %d-> ", atoi(row[5]), atoi(row[4]));

		div_t tmp;
		tmp = div(100 * (reading - atoi(row[5])), ( atoi(row[4]) - atoi(row[5])));
		sprintf(strCalc, "%d", 100 - tmp.quot);
//		printf("%d %d %d %d ", reading,100 - tmp.quot, atoi(row[4]) - atoi(row[5]), reading - atoi(row[5]));

		mysql_free_result(result); 
		result = NULL; 
	}
	else
	{
		printf("Intet mysql resultat. Calculated er derfor tom. -> ");
		sprintf(strCalc, "%d", 0);
	}

	char strReading[20];
	sprintf(strReading, "%d", reading);

	strcpy(strSQL,"INSERT INTO sensorreading (sensor_id, tank_id, ts, reading, calculated) VALUES (1, 1, NOW(),");
	strcat(strSQL,strReading);
	strcat(strSQL,", ");
	strcat(strSQL,strCalc);
	strcat(strSQL,");");
	mysql_query(&mysql,strSQL);

	printf("%d\n",reading);
}

void tempReading(void)
{
	char strSQL[250];
	char strCalc[20];
	char strReading[20];
	char strOk[20] = "N";
	FILE *fp;
	char *line = NULL;
	char *strSplit;
	size_t len = 0;
	ssize_t read;

	fp = fopen("/sys/bus/w1/devices/28-000004e15bbb/w1_slave", "r");
	
	if (fp == NULL)
	{
		printf("ERROR temperatur file didnt open correct!\n");
	
	} else {

		if ((read = getline(&line, &len, fp)) != -1) {
			strSplit = strtok(line, "=");	
			strSplit = strtok(NULL, "=");
			strSplit = strtok(strSplit, " ");
			strSplit = strtok(NULL, " ");
			strcpy(strOk,strSplit);
		}

		if ((read = getline(&line, &len, fp)) != -1) {
			strSplit = strtok(line, "=");	
			strSplit = strtok(NULL, "=");	
			strcpy(strReading,strSplit);
		}
		if(strOk[0] == *("Y"))
		{
			float tmp;
			tmp = atof(strReading);
			sprintf(strReading, "%d", atoi(strReading));
			tmp = tmp / 1000;
			sprintf(strCalc, "%f", tmp);
			printf("Temp %s C %s C",  strReading,  strCalc);
			printf("\n");
			
			strcpy(strSQL,"INSERT INTO sensorreading (sensor_id, tank_id, ts, reading, calculated) VALUES (3, 1, NOW(),");
			strcat(strSQL,strReading);
			strcat(strSQL,", ");
			strcat(strSQL,strCalc);
			strcat(strSQL,");");
			mysql_query(&mysql,strSQL);
	
		} else {
			printf("NO READING\n");
		
		}
		// memory has to bee freed up again
		free(strSplit);
		free(strCalc);
		free(strSQL);
		free(strReading);
		free(strOk);
		free(line);
		fclose(fp);
			   
	}
}

void phReading(void)
{
	char strSQL[500];
	int count ;
	unsigned int nextTime ;
	int check_init = 0;
	int charCount = 0;
	char strPH[10];
	float tmp;

	for (count = 0 ; count < 3 ; )
	{
		printf ("pH read: %3d: ", count) ;
		fflush (stdout) ;
		serialPuts (fd, "R\r") ;
		++count ;
		delay (1000) ;
		charCount = 0;
		while (serialDataAvail (fd))
		{
			strPH[charCount] =  serialGetchar (fd);
			printf ("%c",strPH[charCount]) ;
			charCount++;
			fflush (stdout) ;
		}
		printf("\n");
	}
	
	strcpy(strSQL,"INSERT INTO sensorreading (sensor_id, tank_id, ts, reading, calculated) VALUES (4, 1, NOW(),");
	tmp = atof(strPH);
	sprintf(strPH, "%f", tmp);
	strcat(strSQL,strPH);
	strcat(strSQL,", ");
	strcat(strSQL,strPH);
	strcat(strSQL,");");
//	printf("%s\n",strSQL);
	mysql_query(&mysql,strSQL);
	
	// memory has to bee freed up again
	free(strSQL);
	free(strPH);
	
}

void co2calulation(void)
{
	char strSQL[500];
	char strCO2[50];
	int count ;
	unsigned int nextTime ;
	int check_init = 0;
	int charCount = 0;
	float tmp;
	float lastKH;
	float lastTemp;
	float lastPH;
	char strCalc[20];
	MYSQL_RES *result;
	
// read KH from sensor 6 last input
	strcpy(strSQL, "SELECT calculated FROM sensorreading where sensor_id = 6 order by ts desc limit 0,1;");
	mysql_query(&mysql,strSQL);
	result = mysql_store_result(&mysql);
	if(result){
		MYSQL_ROW row = mysql_fetch_row(result);
		lastKH = atof(row[0]);
	}
// read PH from sensor 4 last input
	strcpy(strSQL, "SELECT calculated FROM sensorreading where sensor_id = 4 order by ts desc limit 0,1;");
	mysql_query(&mysql,strSQL);
	result = mysql_store_result(&mysql);
	if(result){
		MYSQL_ROW row = mysql_fetch_row(result);
		lastPH = atof(row[0]);
	}
// read temp from sensor 3 last input
	strcpy(strSQL, "SELECT calculated FROM sensorreading where sensor_id = 3 order by ts desc limit 0,1;");
	mysql_query(&mysql,strSQL);
	result = mysql_store_result(&mysql);
	if(result){
		MYSQL_ROW row = mysql_fetch_row(result);
		lastTemp = atof(row[0]);
	}
	
	tmp = 6.56260603 * pow(0.99869335, lastTemp);
	tmp = 15.664 * lastKH * pow(10, (tmp-lastPH));
	
	strcpy(strSQL,"INSERT INTO sensorreading (sensor_id, tank_id, ts, reading, calculated) VALUES (5, 1, NOW(),");
	sprintf(strCO2, "%f", tmp);
	strcat(strSQL,strCO2);
	strcat(strSQL,", ");
	strcat(strSQL,strCO2);
	strcat(strSQL,");");
	printf("CO2 reading: %s\n",strCO2);
	mysql_query(&mysql,strSQL);
}


void calculate_output(void)
{
	

}

void hippieShake(void)
{
	int z, q, dataPtr, bit_index, bit;
	printf("\n\nHippie SHAKE\n\n");
	for(q = 0 ; q < 5 ; q++){
		for(z = 0; z < 8; z++)
		{
			dataPtr = exp2(z);
			for (bit_index = 7; bit_index >= 0; --bit_index)
			{
				bit = (dataPtr >> bit_index) & 1;
				digitalWrite(outletDataPin,(bit == 1 ? 0 : 1));
				delay(1);
				digitalWrite(outletClockPin,1);
				delay(1);
				digitalWrite(outletClockPin,0);
				delay(1);
			}
			digitalWrite(outletLatchPin,1);
				delay(1);
			digitalWrite(outletLatchPin,0);
			delay(100);
		}

		for(z = 8; z >= 0; z--)
		{
			dataPtr = exp2(z);
			for (bit_index = 7; bit_index >= 0; --bit_index)
			{
				bit = (dataPtr >> bit_index) & 1;
				digitalWrite(outletDataPin,(bit == 1 ? 0 : 1));
				delay(1);
				digitalWrite(outletClockPin,1);
				delay(1);
				digitalWrite(outletClockPin,0);
				delay(1);
			}
			digitalWrite(outletLatchPin,1);
			delay(1);
			digitalWrite(outletLatchPin,0);
			delay(100);
		}
	}
	for (bit_index = 7; bit_index >= 0; --bit_index)
	{
		digitalWrite(outletDataPin,0);
		delay(1);
		digitalWrite(outletClockPin,1);
		delay(1);
		digitalWrite(outletClockPin,0);
		delay(1);
	}
	digitalWrite(outletLatchPin,1);
	delay(1);
	digitalWrite(outletLatchPin,0);
}

void update_ouput(void)
{
	int z;
	for(z = 0; z < 8; z++)
	{
		digitalWrite(outletDataPin,outlet[z]);
 		delay(1);
		digitalWrite(outletClockPin,1);
		delay(1);
		digitalWrite(outletClockPin,0);
		delay(1);
	}
	
	digitalWrite(outletLatchPin,1);
	delay(1);
	digitalWrite(outletLatchPin,0);
	delay(1);
	printf("Outlet status  -> %d:%d:%d:%d:%d\n", outlet[1], outlet[2], outlet[3], outlet[4], outlet[5]);

	if(outlet[1] == 0 && outlet[2] == 0 && outlet[3] == 0 && outlet[3] == 0 && outlet[5] == 0 )
	{
		// hippieShake();
	}
}



void readmysql(void){

MYSQL_ROW row;

/*	mysql_query(&mysql,"SELECT * FROM sensor");
	MYSQL_RES *result = mysql_store_result(&mysql);
	if(result){
		while(row = mysql_fetch_row(result)){
			sensorActive[(int) row[0]] = atoi(row[3]);
			maxValues[(int) row[0]] = atof(row[4]);
			minValues[(int) row[0]] = atof(row[5]);
		}
		mysql_free_result(result); 
		result = NULL;
 	}
*/
	
	mysql_query(&mysql,"SELECT * FROM outlet WHERE tank_id = 1");
	MYSQL_RES *result = mysql_store_result(&mysql);
	if(result){
		while(row = mysql_fetch_row(result)){
			outlet[atoi(row[0])] = atoi(row[3]);
		}
		mysql_free_result(result); 
		result = NULL; 
	}

}


void initPi(void){
// GPIO initialisering
	int pin;
	if(wiringPiSetup() == -1) exit(1);

	pinMode(outletDataPin, OUTPUT);
	digitalWrite(outletDataPin, 0);	
	pinMode(outletClockPin, OUTPUT);
	digitalWrite(outletDataPin, 0);	
	pinMode(outletLatchPin, OUTPUT);
	digitalWrite(outletDataPin, 0);	

	
// MYSQL initialisering
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql,"localhost","poseidonUSER","poseidon","poseidondb",0,NULL,0))
	{
		fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
	}
	printf("SUCCES :) to connect to database: \n");
	readmysql();
	update_ouput();
	
	
}

void main_loop(void)
{
	for(;;)
	{
		printf("READING SENSORS\n");
		//read sensors
		lightReading();
		tempReading();
		phReading();
		co2calulation();
		
		//Read new values from mysql
		printf("READING MySQL\n");
		readmysql();
		
		//update output switches
		printf("UPDATING OUTPUT\n");
		update_ouput();
		
		//wait a while
		delay(5000);
		printf("\n");
	}

}

int main(void)
{
	initPi();
	if ((fd = serialOpen ("/dev/ttyAMA0", 38400)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	getMac();
	printf("%d:%d:%d:%d:%d:%d\n", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
	
	readmysql();
	update_ouput();
	main_loop();
}

