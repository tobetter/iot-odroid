iot-raspberrypi
===============
This repository contains working sample device client software for the IBM Internet of Things Cloud Quickstart service
on the Raspberry Pi model B.

Raspberry Pi
============

See this [recipe](https://www.ibmdw.net/iot/recipes/raspberry-pi/) for running the code in this repository.


Content
=======
There are two samples:
* c, this sample installs a 'iot' service, which periodically sends events to the internet of things cloud.

* nodered, this sample installs Node-RED (see nodered.org) as an "always on" service. Node-RED provides a capability to develop an application using a browser based UI. The sample recipe installs node for IBM Internet of Things Cloud Quickstart service, together with a sample flow that sends the events up to the Quickstart on a 1s interval.


