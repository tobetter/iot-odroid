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
#include "iot.h"
#include "MQTTAsync.h"

char propFileLoc[20] = "./iot.properties";
char lockFileLoc[10] = "./.lck";
char pidFileLoc[10] = "./.pid";
float PI = 3.1415926;
float MIN_VALUE = -50.0;
float MAX_VALUE = 50.0;

char clientId[MAXBUF];
char topic[MAXBUF];

MQTTAsync client;

struct config {
	char hostname[MAXBUF];
	char timeout[MAXBUF];
	char tenantprefix[MAXBUF];
};

float getCPUTemp();
float GetCPULoad();
char *getmac(char *iface);
void get_config(char* filename, struct config * configstr);
char *getClientId(char* tenant_prefix, char* mac_address);
char *getTopic(char* ext_device_id);
float sineVal(float minValue, float maxValue, float duration, float count);
char * generateJSON(JsonMessage passedrpi );

int init_mqtt_connection(MQTTAsync* client, char *address, char* client_id);
int publishMQTTMessage(MQTTAsync* client, char *topic, char *payload);
int disconnect_mqtt_client(MQTTAsync* client);

int main(int argc, char **argv) {
	printf("***********Entering the IOT raspberry pi device********************\n");

	struct config configstr;
	char* json;

	int lckStatus;
	int res;
	int sleepTimeout;

	float temp = getCPUTemp();
	char* mac_address = getmac("eth0");
	//first check if there is another process already started
	if (access(lockFileLoc, F_OK) != -1) {
		printf("There is another process of IOT-Raspberry running. Please close that before starting. If there are no other IOT-Raspberry process running, please delete the .lck file and start again\n");
		exit(1);
	} else {
		FILE *fp;
		fp = fopen(lockFileLoc, "w");
		printf("Creating the .lck file\n");
		fclose(fp);
	}
	
	// to load the config files
	get_config(propFileLoc, &configstr);
	getClientId(configstr.tenantprefix, mac_address);
	getTopic(mac_address);

	// initialize the MQTT connection
	init_mqtt_connection(&client, configstr.hostname, clientId);
	// Wait till we get a successful connection to IoT MQTT server
	while(connected != 1) {
		if(connected == -1) {
			printf("Retry connection\n",connected);
			connected = 0;
			init_mqtt_connection(&client, configstr.hostname, clientId);
		}
		sleep(1);
	}	

	// count for the sine wave
	int count = 1;
	sleepTimeout = atoi(configstr.timeout);
	while(1){
		JsonMessage json_message = {"myPi", getCPUTemp(), sineVal(MIN_VALUE, MAX_VALUE, 5 , count), GetCPULoad()};
		json = generateJSON(json_message);
		res = publishMQTTMessage(&client, topic, json);
		printf("Posted the message with result code = %d\n",res);
		fflush(stdout);
		free(json);
		count++;
		sleep(sleepTimeout);
	}

	//disconnect the client
	disconnect_mqtt_client(&client);
	return 0;
}
// This is the function to read the config from the iot.properties file
void get_config(char * filename, struct config * configstr) {

	FILE* prop;
	char str1[10], str2[10];
	prop = fopen(filename, "r");
	if( prop == NULL )
   	{
      		perror("Error while opening the properties file. Please ensure that the properties file exist in this directory\n");
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
			fprintf(stderr, "Syntax error, line %d\n", linenum);
			continue;
		}
		if (strcmp(prop, "url") == 0)
			strncpy(configstr->hostname, value, MAXBUF);
		else if (strcmp(prop, "timeinterval") == 0)
			strncpy(configstr->timeout, value, MAXBUF);
		else if (strcmp(prop, "tenantprefix") == 0)
			strncpy(configstr->tenantprefix, value, MAXBUF);

	}

}

//This generates the clientID based on the tenant_prefix and mac_address(external Id)
char *getClientId(char* tenant_prefix, char* mac_address) {

	sprintf(clientId, "%s:%s",tenant_prefix,mac_address);
	return clientId;
}

char *getTopic(char* ext_device_id) {

	sprintf(topic, "iot-1/d/%s/evt/rpi-quickstart/json",ext_device_id);
        return topic;
}

//This function generates the sine value based on the interval specified and the duration
float sineVal(float minValue, float maxValue, float duration, float count) {
        float sineValue;
        sineValue = sin(2.0 * PI * count / duration) * (maxValue - minValue) / 2.0; 
        return sineValue;
}

