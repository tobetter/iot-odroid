#!/bin/sh

devId=`/sbin/ifconfig | grep 'eth0' | tr -s ' ' | cut -d ' ' -f5 |  tr -d ':'`

echo The device ID is $devId
echo For Real-time visualization of the data, visit http://quickstart.internetofthings.ibmcloud.com/?deviceId=$devId


