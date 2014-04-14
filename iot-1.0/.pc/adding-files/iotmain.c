#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "iot.h"
#define MAXBUF 100

char propFileLoc[20] = "./iot.properties";
char lockFileLoc[10] = "./.lck";
char pidFileLoc[10] = "./.pid";
float PI = 3.1415926;
float MIN_VALUE = -50.0;
float MAX_VALUE = 50.0;


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
int publishMQTTMessage(char *address, char* client_id, char *topic, char *payload);
char *getTopic(char* ext_device_id);
float sineVal(float minValue, float maxValue, float duration, float count);
char * generateJSON(JsonMessage passedrpi );

int main(int argc, char **argv) {
	printf("***********Entering the IOT raspberry pi device********************\n");

	struct config configstr;
	char* clientId;
	char* topic;
	char* json;

	int lckStatus;
	int res;

	float temp = getCPUTemp();
	char* mac_address = getmac("eth0");

	//first check if there is another process already started
	if (access(lockFileLoc, F_OK) != -1) {
		printf("There is another process of IOT-Raspberry running. Please close that before starting. If there are no other IOT-Raspberry process running, please delete the .lck file and start again\n");
	} else {
		FILE *fp;
		fp = fopen(lockFileLoc, "w");
		printf("Creating the .lck file\n");
		fclose(fp);
	}
	
	// to load the config files
	get_config(propFileLoc, &configstr);
	clientId = getClientId(configstr.tenantprefix, mac_address);
	topic = getTopic(mac_address);
	// count for the sine wave
	int count = 1;
	while(1){
		/*	json_message.cputemp = getCPUTemp();
		json_message.cpuload = GetCPULoad();
		json_message.sine = sineVal(MIN_VALUE, MAX_VALUE, 5 , count);
		strcpy(json_message.myname, "mypi");*/
		JsonMessage json_message = {"myPi", getCPUTemp(), sineVal(MIN_VALUE, MAX_VALUE, 5 , count), GetCPULoad()};
		json = generateJSON(json_message);
		printf("THe json is %s",json);
		res = publishMQTTMessage(configstr.hostname, clientId, topic, json);
		printf("Posted the message with result code = %d",res);
		fflush(stdout);
		count++;
		sleep(atoi(configstr.timeout));
	}
	return 0;
}

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
		if (strcmp(prop, "hostname") == 0)
			strncpy(configstr->hostname, value, MAXBUF);
		else if (strcmp(prop, "timeout") == 0)
			strncpy(configstr->timeout, value, MAXBUF);
		else if (strcmp(prop, "tenantprefix") == 0)
			strncpy(configstr->tenantprefix, value, MAXBUF);

	}

}

char *getClientId(char* tenant_prefix, char* mac_address) {
	char* clientid;

	clientid = strcat(tenant_prefix, ":");
	clientid = strcat(clientid, mac_address);

	return clientid;
}

char *getTopic(char* ext_device_id) {
        char* topic;

        strcpy(topic, "iot-1/d/");
        strcat(topic,ext_device_id);
	topic = strcat(topic, "/evt/rpi-quickstart/json"); 

        return topic;
}

float sineVal(float minValue, float maxValue, float duration, float count) {
        float sineValue;
        sineValue = sin(2.0 * PI * count / duration) * (maxValue - minValue) / 2.0; 
        return sineValue;
}

