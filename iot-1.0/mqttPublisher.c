/*******************************************************************************
 * Copyright (c) 2014 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: rajathr1
 * IBM - Initial Contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"

#define DEBUG 1 //make this 1 for debugging
#ifdef DEBUG
#define ERROR 1
#define WARN 1
#define INFO 1
#define FINE 1
#else
#define ERROR 1
#define WARN 1
#define INFO 0
#define FINE 0
#endif

#define QOS 0

/* this maintains the status of connection
 *  0 - Not connected
 *  1 - Connected
 * -1 - Connection Failed, retry
 */
int connected = 0;
volatile MQTTAsync_token deliveredtoken;

/*
 * Try to reconnect if the connection is lost 
 */
void connlost(void *context, char *cause) {
	MQTTAsync client = (MQTTAsync) context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;
#ifdef ERROR
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
#endif

#ifdef INFO
	printf("Reconnecting\n");
#endif
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
#ifdef ERROR
		printf("Failed to start connect, return code %d\n", rc);
#endif
	}
}

/*
 * This function is called when the message is successfully published
 */
void onSend(void* context, MQTTAsync_successData* response) {

#ifdef FINE
	printf("Event with token value %d delivery confirmed\n", response->token);
#endif
}

/*
 * Called when the connection is successful. Update the connected variable
 */
void onConnectSuccess(void* context, MQTTAsync_successData* response) {

#ifdef FINE
	printf("Successful connection\n");
#endif
	// The connection is successful. update it to 1
	connected = 1;

}

/*
 * After sending the message disconnect from IoT/mqtt server
 */
int disconnect_mqtt_client(MQTTAsync* client) {
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc = MQTTASYNC_SUCCESS;

	opts.context = client;

	if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS) {
#ifdef ERROR
		printf("Failed to start sendMessage, return code %d\n", rc);
#endif
	}
	MQTTAsync_destroy(client);
	return rc;
}

/* 
 * On failure of connection to server, print the response code 
 */
void onConnectFailure(void* context, MQTTAsync_failureData* response) {
#ifdef ERROR
	printf("Connect failed ");
	if(response) {
		printf("with response code : %d",response->code);
	} 
#endif
	connected = -1; // connection has failed
}

/* 
 * Function is used to initialize the MQTT connection handle "client"
 */
int init_mqtt_connection(MQTTAsync* client, char *address, char* client_id) {

	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc = MQTTASYNC_SUCCESS;
	MQTTAsync_create(client, address,client_id , MQTTCLIENT_PERSISTENCE_NONE,
			NULL);

	MQTTAsync_setCallbacks(*client, NULL, connlost, NULL, NULL);

#ifdef INFO
	printf("Connecting to %s with client Id: %s \n", address, client_id);
#endif
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnectSuccess;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	if ((rc = MQTTAsync_connect(*client, &conn_opts)) != MQTTASYNC_SUCCESS) {
#ifdef ERROR
		printf("Failed to start connect, return code %d\n", rc);
#endif
	}
	return rc;
}
/* 
 * Function is used to publish events to the IoT MQTT reciver. 
 * This reuses the "client"  
 */
int publishMQTTMessage(MQTTAsync* client, char *topic, char *payload) {

	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	int rc = MQTTASYNC_SUCCESS;

	opts.onSuccess = onSend;
	opts.context = *client;

	pubmsg.payload = payload;
	pubmsg.payloadlen = strlen(payload);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	deliveredtoken = 0;

	if ((rc = MQTTAsync_sendMessage(*client, topic, &pubmsg, &opts))
			!= MQTTASYNC_SUCCESS) {
#ifdef ERROR
		printf("Failed to start sendMessage, return code %d\n", rc);
#endif
		return rc;
	}

#ifdef INFO
	printf("Waiting for publication of %s\n"
			"on topic %s\n", payload, topic);
#endif

	return rc;
}

