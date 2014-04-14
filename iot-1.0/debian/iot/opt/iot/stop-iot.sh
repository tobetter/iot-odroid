#!/bin/sh

pid_file=./.pid
lck_file=./.lck

if [ -e "$pid_file" ]
then
	pid="`cat $pid_file`"
	echo "Stopping the IoT-Raspberry Pi program with pid : $pid" >> iot.log
	echo "*****************************************************" >> iot.log
	echo "Stopping the IoT-Raspberry Pi program with pid : $pid"
	kill -9 $pid
	rm $pid_file
	rm $lck_file 
else
	echo "The IoT-Raspberry process is not started yet. Please use start-iot.sh to start"
fi

