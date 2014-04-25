iot-raspberrypi
===============
This is the sample program to send the following events from the Raspberry Pi to the Quickstart IBM Internet of Things service. 

The events that are emitted in this sample are:

1. CPU temperature
2. CPU Load
3. Simulated Sine wave to demonstrate the different events can be pushed to IoT Portal and visualized.

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



### Visualize

You can now use the IoT web site to visualize the data that your Raspberry Pi is generating. 

Run `iotGetDeviceID.sh` to get the device ID(Mac address) for your Raspberry Pi

```
pi@raspberrypi /opt/iot $ ./iotGetDeviceID.sh
b827eba84426
```

* Enter the Device Id in the portal page
* Now you will see a visual representation of the events that was sent from the Raspberry Pi.



### Other Commands

#### Stop
```
pi@raspberrypi /opt/iot $ sudo invoke-rc.d iot stop
Stopping the program
Stopping the IoT-Raspberry Pi program with pid : <pid>
```

#### Start
```
pi@raspberrypi /opt/iot $ sudo invoke-rc.d iot start
Starting the iot program
```

####Uninstall
```
pi@raspberrypi ~ $ sudo dpkg -P iot
(Reading database ... 68615 files and directories currently installed.)
Removing iot ...
Stopping the program
Stopping the IoT-Raspberry Pi program with pid : 8592
Purging configuration files for iot ...
```

IoT process logs with the help of syslog. So to view the logs, go to /var/log/syslog. 