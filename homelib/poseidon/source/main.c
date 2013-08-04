// Version 1.01 - Git update 1
// by KeikoWare

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <stdint.h>
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
//GPIO pin assignment
//pH reading uses serial connection
// RXD & TXD are asigned to the pH probe board TX & RX
// int serialTXDpin = 15;  // GPIO pin 14
// int serialRXDpin = 16;  // GPIO pin 15

//Temperature reading uses Dallas 1 Wire connection
// int dalasOneWire = 7;   // GPIO pin 04 

//Light measurement uses one pin to measure light
int lightPin = 0;       // GPIO pin 17

//Relay control are controlled by shift register control
int outletDataPin  = 1; // GPIO pin 18 -> DS
int outletClockPin = 4; // GPIO pin 23 -> SHCP
int outletLatchPin = 5; // GPIO pin 24 -> STCP


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
	
	mysql_free_result(result); 
	result = NULL; 
}

void tempReading(void)
{
	char strSQL[250];
	char strCalc[20];
	char strReading[20];
	char strOk[20] = "N";
	float tmp;
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
		// free(strSplit);
		// attempt to free a nonheap object // free(strCalc);
		// attempt to free a nonheap object // free(strSQL);
		// attempt to free a nonheap object // free(strReading);
		// attempt to free a nonheap object // free(strOk);
		free(line);
		fclose(fp);
		// free(fp);
			   
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
	int fd ;

	// serial initialisering
	if ((fd = serialOpen ("/dev/ttyAMA0", 38400)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
        exit(EXIT_FAILURE);
	}
	
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
	serialClose(fd);
	// free(fd);
	
	// memory has to bee freed up again
	// attempt to free a nonheap object // free(strSQL);
	// attempt to free a nonheap object // free(strPH);
	
}

void co2calulation(void)
{
	char strSQL[500];
	char strCO2[50];
	int count ;
	int check_init = 0;
	int charCount = 0;
	float tmp;
	float lastKH;
	float lastTemp;
	float lastPH;
	MYSQL_RES *result;
	
// read KH from sensor 6 last input
	strcpy(strSQL, "SELECT calculated FROM sensorreading where sensor_id = 6 order by ts desc limit 0,1;");
	mysql_query(&mysql,strSQL);
	result = mysql_store_result(&mysql);
	if(result){
		MYSQL_ROW row = mysql_fetch_row(result);
		lastKH = atof(row[0]);
//		free(row); // new line
	}
	mysql_free_result(result); 
// read PH from sensor 4 last input
	strcpy(strSQL, "SELECT calculated FROM sensorreading where sensor_id = 4 order by ts desc limit 0,1;");
	mysql_query(&mysql,strSQL);
	result = mysql_store_result(&mysql);
	if(result){
		MYSQL_ROW row = mysql_fetch_row(result);
		lastPH = atof(row[0]);
//		free(row); // new line
	}
	mysql_free_result(result); 
// read temp from sensor 3 last input
	strcpy(strSQL, "SELECT calculated FROM sensorreading where sensor_id = 3 order by ts desc limit 0,1;");
	mysql_query(&mysql,strSQL);
	result = mysql_store_result(&mysql);
	if(result){
		MYSQL_ROW row = mysql_fetch_row(result);
		lastTemp = atof(row[0]);
//		free(row); // new line
	}
	mysql_free_result(result); 
	
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
	
	// memory has to bee freed up again
	// free(strSQL); // attempt to free a nonheap object
	// free(strCO2); // attempt to free a nonheap object
	//mysql_free_result(result); 
	result = NULL; 

}


void calculate_output(void)
{
	

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
	if(wiringPiSetup() == -1) exit(EXIT_FAILURE);

	pinMode(outletDataPin, OUTPUT);
	digitalWrite(outletDataPin, 0);	
	pinMode(outletClockPin, OUTPUT);
	digitalWrite(outletDataPin, 0);	
	pinMode(outletLatchPin, OUTPUT);
	digitalWrite(outletDataPin, 0);	
	getMac();
	printf("%d:%d:%d:%d:%d:%d\n", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
}

void main_loop(void)
{
	for(;;)
	{
		// MYSQL initialisering
		mysql_init(&mysql);
		if (!mysql_real_connect(&mysql,"localhost","poseidonUSER","poseidon","poseidondb",0,NULL,0))
		{
			fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
			exit(EXIT_FAILURE);
		}
		printf("SUCCES :) to connect to database: \n");
	
		printf("READING SENSORS\n");
		//read sensors
		// to find memory leak we activate one at a time, starting of with all disabled
		lightReading(); // testing
		tempReading(); // testing
		phReading(); // testing
		co2calulation(); // testing
		
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

       // Our process ID and Session ID 
        pid_t pid, sid;
        
        // Fork off the parent process 
        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }
        // If we got a good PID, then
        //  we can exit the parent process.
        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }

        // Change the file mode mask 
        umask(0);
                
        // Open any logs here
                
        // Create a new SID for the child process
        sid = setsid();
        if (sid < 0) {
                // Log the failure 
                exit(EXIT_FAILURE);
        }
        
        // Change the current working directory
        if ((chdir("/")) < 0) {
                // Log the failure
                exit(EXIT_FAILURE);
        }
        
        // Close out the standard file descriptors 
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
        // Daemon-specific initialization goes here 
 
	/* The Big Loop */
	initPi();
	main_loop();

   exit(EXIT_SUCCESS);
}

