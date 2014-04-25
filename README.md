iot-raspberrypi
===============
This is the sample program to send the following events from the Raspberry Pi to the Quickstart IBM Internet of Things service. 

### Events
* CPU Temperature of Raspberry Pi
* CPU load - The average CPU load across a minute
* Sine wave - Simulated Sine wave to demonstrate that different events can be pushed to IoT Portal and visualized.

### Use the following commands to setup and run the sample


####Get the deb installer
```
pi@raspberrypi ~ $ curl -L https://github.com/ibm-messaging/iot-raspberrypi/blob/master/iot_1.0-1_armhf.deb?raw=true > iot_1.0-1_armhf.deb
```

####Install the deb
```
pi@raspberrypi ~ $ sudo dpkg -i iot_1.0-1_armhf.deb
Selecting previously unselected package iot.
(Reading database ... 68603 files and directories currently installed.)
Unpacking iot (from iot_1.0-1_armhf.deb) ...
Setting up iot (1.0-1) ...
Starting the iot program
```

This installs and starts the iot process.  Use the status call to check the status of the process
```
pi@raspberrypi ~ $ invoke-rc.d iot status
[ ok ] iot is running.
```

### Others Commands

#### To stop the service
```
pi@raspberrypi /opt/iot $ sudo invoke-rc.d iot stop
Stopping the program
Stopping the IoT-Raspberry Pi program with pid : <pid>
```

#### To start the service
```
pi@raspberrypi /opt/iot $ sudo invoke-rc.d iot start
Starting the iot program
```

####To uninstall the IoT process
```
pi@raspberrypi ~ $ sudo dpkg -P iot
(Reading database ... 68615 files and directories currently installed.)
Removing iot ...
Stopping the program
Stopping the IoT-Raspberry Pi program with pid : 8592
Purging configuration files for iot ...
```


### Visualize

You can now use the IoT web site to visualize the data that your Raspberry Pi is generating. 

Run `iotGetDeviceID.sh` to get the device ID(Mac address) for your Raspberry Pi

```
pi@raspberrypi /opt/iot $ ./iotGetDeviceID.sh
b827eba84426
```

* Enter the Device Id in the portal page
* Now you will see a visual representation of the events that was sent from the Raspberry Pi.


This sample program will post events emitted from Raspberry Pi and publish it to IoT cloud. 

The events that are emitted in this sample are 

1. CPU temperature
2. CPU Load
3. Simulated Sine wave to demonstrate the different events can be pushed to IoT Portal and visualized.

Steps to run the sample:

1. Run the start script using the following command 
	./start-iot.sh

2. It will print the pid and start the iot process. 

3. If you want to stop the process. Invoke the following command
	./stop-iot.sh
	
	
IoT process logs with the help of syslog. So to view the logs, go to /var/log/syslog. 


IMPORTANT NOTE:
---------------
Please use the start-iot.sh and stop-iot.sh to start and stop the process. Unless you understand the working of this 
sample, do not directly use "iot" executable. 




This repository will contain a sample for connecting a [Raspberry Pi](http://www.raspberrypi.org/) to the IBM Internet of Things Cloud.

See the [Wiki](https://github.com/ibm-messaging/iot-raspberrypi/wiki) for more details of the example.

If you find this example helpful, please Star the repository as that will help us focus on the things that people find the most helpful.

This repository is not yet ready to receive contributions.



IoT-Raspberry Pi README
------------------------

This sample program will post events emitted from Raspberry Pi and publish it to IoT cloud. 

The events that are emitted in this sample are 

1. CPU temperature
2. CPU Load
3. Simulated Sine wave to demonstrate the different events can be pushed to IoT Portal and visualized.

Steps to run the sample:

1. Run the start script using the following command 
	./start-iot.sh

2. It will print the pid and start the iot process. 

3. If you want to stop the process. Invoke the following command
	./stop-iot.sh
	
	
IoT process logs with the help of syslog. So to view the logs, go to /var/log/syslog. 


IMPORTANT NOTE:
---------------
Please use the start-iot.sh and stop-iot.sh to start and stop the process. Unless you understand the working of this 
sample, do not directly use "iot" executable. 



