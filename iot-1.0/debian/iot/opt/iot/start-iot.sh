#!/bin/sh

file_name=~/.lck

if [ -e "$file_name" ]
then
    echo "There is another process of IOT-Raspberry Pi running. Please stop that process before using this script again. If there are no other IOT-Raspberry process running, please delete the .lck file and start again"
exit 1
fi

echo "Starting the IoT-Raspberry"

#cd ./bin
./iot &
pid=$!

echo $pid >~/.pid

echo "Started the IoT-Raspberry. Running with pid : $pid"
