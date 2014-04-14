/*
 * iot.h
 */

#ifndef IOT_H_
#define IOT_H_

/* This is structure for the JSON */

  struct json {
	  char myname[100];
	  float cputemp;
	  float sine;
	  float cpuload;
  };

  /* This is the short hand for json */
  typedef struct json JsonMessage;

#define MAXBUF 100
#endif /* IOT_H_ */
