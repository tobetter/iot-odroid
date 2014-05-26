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
pi@raspberrypi ~ $ curl -LO https://github.com/ibm-messaging/iot-raspberrypi/releases/download/1.0.0/iot1.0.0_armhf.deb
```

####Install the deb
```
pi@raspberrypi ~ $ sudo dpkg -i iot1.0.0_armhf.deb
Selecting previously unselected package iot.
(Reading database ... 68603 files and directories currently installed.)
Unpacking iot (from iot1.0.0_armhf.deb) ...
Setting up iot (1.0-1) ...
Starting the iot program
```

This installs and starts the iot process.  Use the status call to check the status of the process
```
pi@raspberrypi ~ $ invoke-rc.d iot status
[ ok ] iot is running.
```



### Visualize

Visit the [IBM Internet of Things QuickStart Portal](http://quickstart.internetofthings.ibmcloud.com/) and enter your device ID to see real time visualizations of the data sent from your device.

Run `invoke-rc.d iot getdeviceid` to get the device ID (MAC address) for your Raspberry Pi

```
pi@raspberrypi ~ $ invoke-rc.d iot getdeviceid
The device ID is b827eba84426
For Real-time visualization of the data, visit http://quickstart.internetofthings.ibmcloud.com/?deviceId=b827eba84426
```


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


#####Note for Windows Users:
When you clone/fork this repository,ensure that file permissions for the libraries are set properly and that  "libpaho-mqtt3a.so" and "libpaho-mqtt3a.so.1" are symbolic links. 
