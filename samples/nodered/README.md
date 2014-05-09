Node-RED sample
===============

This is the sample program to send the following events from the Raspberry Pi to the Quickstart IBM Internet of Things service.

The events that are emitted in this sample are:

+ CPU temperature
+ CPU Load
+ Simulated Sine wave to demonstrate the different events can be pushed to IoT Portal and visualized.


Connect
-------

1. Log in to Raspberry Pi. (Default login Username: pi Password: raspberry)
2. Download the sample tarball using __wget https://github.com/jeffdare/iot-raspberrypi/releases/download/1.0.0-nodered/iot-1.0.0-nodered.tar.gz__
3. __cd iot-raspi-nodered__
4. Run __sudo ./setup.sh__. This completes the following tasks:
   Installs Node-RED, contributed nodes, npm requisites, and configures a system service to run a supplied flow.
   This will take a few minutes. (This command requires __sudo__ as it registers the Node-RED as a init service)
5. Find your MAC address for example using __./iotGetDeviceID.sh__ which is needed for the [quickstart site](http://quickstart.internetofthings.ibmcloud.com).
6. Start Node-RED with __sudo service node-red start__


Troubleshooting and Development
--------------------------------
+ Check the node-red.out file for diagnostics from Node-RED.
+ You can connect a browser to port [raspberrypi-address:1880] in order to develop the flow in the Node-RED UI, activate debug nodes to show events emitted.
+ If __git__ is not installed, install it using __sudo apt-get install git__
