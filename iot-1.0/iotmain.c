/*******************************************************************************
 * Copyright (c) 2014 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: jeffdare
 * IBM - Initial Contribution
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include "iot.h"
#include "MQTTAsync.h"
#include <syslog.h>

char propFileLoc[20] = "./iot.properties";
char lockFileLoc[20];
float PI = 3.1415926;
float MIN_VALUE = -50.0;
float MAX_VALUE = 50.0;

char clientId[MAXBUF];
char topic[MAXBUF];

MQTTAsync client;

struct config {
	char hostname[MAXBUF];
	char timeout[10];
	char tenantprefix[MAXBUF];
	char loglevel[2];
};

void get_config(char* filename, struct config * configstr);
char *getClientId(char* tenant_prefix, char* mac_address);
char *getTopic(char* ext_device_id);
float sineVal(float minValue, float maxValue, float duration, float count);
void sig_handler(int signo);
int reconnect_delay(int i);

//cpustat.c
float getCPUTemp();
float GetCPULoad();
//mac.c
char *getmac(char *iface);
//jsonator.c
char * generateJSON(JsonMessage passedrpi);

//mqttPublisher.c
int init_mqtt_connection(MQTTAsync* client, char *address, char* client_id);
int publishMQTTMessage(MQTTAsync* client, char *topic, char *payload);
int disconnect_mqtt_client(MQTTAsync* client);
int reconnect(MQTTAsync* client);

int main(int argc, char **argv) {

	// Removing the properties file and reading all the values from the typedefs in iot.h
	//struct config configstr;
	char* json;

	int lckStatus;
	int res;
	int sleepTimeout;
printf("%s",lockFileLoc);
	// to load the config files
	//get_config(propFileLoc, &configstr);

	//setup the syslog logging
	setlogmask (LOG_UPTO (LOGLEVEL));
	openlog("iot",LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "**** IoT Raspberry Pi Sample has started ****");

	// register the signal handler for USR1-user defined signal 1
	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
		syslog(LOG_CRIT,"Not able to register the signal handler\n");
	if (signal(SIGINT, sig_handler) == SIG_ERR)
                syslog(LOG_CRIT,"Not able to register the signal handler\n");

	// read the events
	char* mac_address = getmac("eth0");
	getClientId(TENANT_PREFIX, mac_address);
	getTopic(mac_address);

	//the timeout between the connection retry
	int connDelayTimeout = 1;	// default sleep for 1 sec
	int retryAttempt= 0;

	// initialize the MQTT connection
	init_mqtt_connection(&client, MSPROXY_URL, clientId);
	// Wait till we get a successful connection to IoT MQTT server
	while (!MQTTAsync_isConnected(client)) {
		connDelayTimeout = 1; // add extra delay(3,60,600) only when reconnecting
		if (connected == -1) {
			connDelayTimeout = reconnect_delay(++retryAttempt);	//Try to reconnect after the retry delay
			syslog(LOG_ERR,"Failed connection attempt #%d. Will try to reconnect "
					"in %d seconds\n", retryAttempt, connDelayTimeout);
			connected = 0;
			init_mqtt_connection(&client, MSPROXY_URL, clientId);
		}
		fflush(stdout);
		sleep(connDelayTimeout);
	}
	// resetting the counters
	connDelayTimeout = 1;
	retryAttempt= 0;

	// count for the sine wave
	int count = 1;
	sleepTimeout = EVENTS_INTERVAL;
	while (1) {
		JsonMessage json_message = { DEVICE_NAME, getCPUTemp(), sineVal(MIN_VALUE,
				MAX_VALUE, 5, count), GetCPULoad() };
		json = generateJSON(json_message);
		res = publishMQTTMessage(&client, topic, json);
		syslog(LOG_DEBUG,"Posted the message with result code = %d\n", res);
		if (res == -3) {
			//update the connected to connection failed
			connected = -1;
			while (!MQTTAsync_isConnected(client)) {
				if(connected == -1) {
					connDelayTimeout = reconnect_delay(++retryAttempt);     //Try to reconnect after the retry delay
					syslog(LOG_ERR,"Failed connection attempt #%d. "
                        				"Will try to reconnect in %d "
                        				"seconds\n", retryAttempt, connDelayTimeout);
					sleep(connDelayTimeout);
					connected = 0;
					reconnect(&client);
				}
				fflush(stdout);
				sleep(1);
			}
		 	// resetting the counters
			connDelayTimeout = 1;
			retryAttempt= 0;
		}
		fflush(stdout);
		free(json);
		count++;
		sleep(sleepTimeout);
	}

	return 0;
}
// This is the function to read the config from the iot.properties file
void get_config(char * filename, struct config * configstr) {

	FILE* prop;
	char str1[10], str2[10];
	prop = fopen(filename, "r");
	if (prop == NULL) {
		syslog(LOG_CRIT,
				"Error while opening the properties file. Please ensure "
				"that the properties file exist in this directory\n");
		exit(1);
	}
	char line[256];
	int linenum = 0;
	while (fgets(line, 256, prop) != NULL) {
		char prop[256], value[256];

		linenum++;
		if (line[0] == '#')
			continue;

		if (sscanf(line, "%s %s", prop, value) != 2) {
			syslog(LOG_ERR, "Syntax error in properties file, line %d\n", linenum);
			continue;
		}
		if (strcmp(prop, "url") == 0)
			strncpy(configstr->hostname, value, MAXBUF);
		else if (strcmp(prop, "timeinterval") == 0)
			strncpy(configstr->timeout, value, 10);
		else if (strcmp(prop, "tenantprefix") == 0)
			strncpy(configstr->tenantprefix, value, MAXBUF);
		else if (strcmp(prop, "loglevel") == 0)
					strncpy(configstr->loglevel, value, 10);

	}

}

//This generates the clientID based on the tenant_prefix and mac_address(external Id)
char *getClientId(char* tenant_prefix, char* mac_address) {

	sprintf(clientId, "%s:%s", tenant_prefix, mac_address);
	return clientId;
}

char *getTopic(char* ext_device_id) {

	sprintf(topic, "iot-1/d/%s/evt/rpi-quickstart/json", ext_device_id);
	return topic;
}

//This function generates the sine value based on the interval specified and the duration
float sineVal(float minValue, float maxValue, float duration, float count) {
	float sineValue;
	sineValue = sin(2.0 * PI * count / duration) * (maxValue - minValue) / 2.0;
	return sineValue;
}

// Signal handler to handle when the user tries to kill this process. Try to close down gracefully
void sig_handler(int signo) {
	syslog(LOG_INFO,"Received the signal to terminate the IoT process. \n");
	syslog(LOG_INFO,"Trying to end the process gracefully. Closing the MQTT connection. \n");
	int res = disconnect_mqtt_client(&client);

	syslog(LOG_INFO,"Disconnect finished with result code : %d\n", res);
	syslog(LOG_INFO,"Shutdown the process is complete. \n");
	syslog(LOG_INFO,
			"**** IoT Raspberry Pi Sample has ended ****");
	closelog();
	exit(1);
}
/* Reconnect delay time 
 * depends on the number of failed attempts
 */
int reconnect_delay(int i) {
        if(i<10) {
                return 3; // first 10 attempts try within 3 seconds
        }
        if(i < 20)
                return 60; // next 10 attempts retry after every 1 minute

        return 600;	// after 20 attempts, retry every 10 minutes
}
