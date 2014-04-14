#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"

#define DEBUG 0 //make this 1 for debugging
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

volatile MQTTAsync_token deliveredtoken;

int finished = 0;
char *message_payload;
char *publish_topic;
/*
 * Try to reconnect if the connection is lost 
 */
void connlost(void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
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
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
#ifdef ERROR
		printf("Failed to start connect, return code %d\n", rc);
#endif
 		finished = 1;
	}
}

/*
 * Once messaged is sent and disconnected, update the finished variable
 */
void onDisconnect(void* context, MQTTAsync_successData* response)
{
#ifdef FINE
	printf("Successful disconnection\n");
#endif
	finished = 1;
}

/*
 * After sending the message disconnect from IoT/mqtt server
 */
void onSend(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

#ifdef FINE
	printf("Message with token value %d delivery confirmed\n", response->token);
#endif

	opts.onSuccess = onDisconnect;
	opts.context = client;

	if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
	{
#ifdef ERROR
		printf("Failed to start sendMessage, return code %d\n", rc);
#endif
		exit(-1);	
	}
}

/* 
 * On failure of connection to server, print the response code 
 */
void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
#ifdef ERROR
	printf("Connect failed, rc %d\n", response ? response->code : 0);
#endif
	finished = 1;
}

/*
 * After establishing the connection, publish the message to predefined TOPIC
 */
void onConnect(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;
#ifdef FINE
	printf("Successful connection\n");
#endif
	
	opts.onSuccess = onSend;
	opts.context = client;

	pubmsg.payload = message_payload;
	pubmsg.payloadlen = strlen(message_payload);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	deliveredtoken = 0;

	if ((rc = MQTTAsync_sendMessage(client, publish_topic, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
#ifdef ERROR
		printf("Failed to start sendMessage, return code %d\n", rc);
#endif
 		exit(-1);	
	}
}

int publishMQTTMessage(char *address, char* client_id, char *topic, char *payload) {

	MQTTAsync client;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	
	int rc;
	message_payload = payload;
	publish_topic = topic;

	MQTTAsync_create(&client, address, client_id, MQTTCLIENT_PERSISTENCE_NONE, NULL);

	MQTTAsync_setCallbacks(client, NULL, connlost, NULL, NULL);

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	conn_opts.onSuccess = onConnect;
	conn_opts.onFailure = onConnectFailure;
	conn_opts.context = client;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
#ifdef ERROR
		printf("Failed to start connect, return code %d\n", rc);
#endif
		exit(-1);	
	}

#ifdef INFO
	printf("Waiting for publication of %s\n"
         "on topic %s for client with ClientID: %s\n",
         payload, topic, client_id);
#endif
	while (!finished)
		usleep(10000L);

	MQTTAsync_destroy(&client);
 	return rc;
}

  
