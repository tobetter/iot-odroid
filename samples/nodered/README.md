Node-RED sample
===============

Connect
-------

1. Log in to Raspberry Pi. 
2. Clone the GitHub project: __git clone https://github.com/ibm-messaging/iot-raspberrypi__
3. __cd iot-raspberrypi/samples/nodered__
4. Run __./setup.sh__. This completes the following tasks:
   Installs node-red, contributed nodes, npm requisites, and configures a system service to run a supplied flow.
   This will take a few minutes.
5. Find your MAC address for example using __./iotGetDeviceID.sh__ which is needed for the [quickstart site](http://quickstart.internetofthings.ibmcloud.com).
6. Start node-red with __service node-red start__


Troubleshooting and Development
---------------------------
+ Check the node-red.out file for diagnostics from node-red.
+ You can connect a browser to port [raspberrypi-address:1880] in order to develop the flow in the Node-RED UI, activate debug nodes to show events emitted.
