# ChilIO

This is a little IoT project to monitor moisture levels and other parameters of (chili) plants, and automate the watering.

![BlockDiagram](doc/general-structure.svg)

## Hardware

To measure the moisture level, some simple resistive moisture sensors are used. To enable/disable water flow, solenoid valves are used, which require a little circuit to drive from a microcontroller.

Later I would also like to add a flow meter, temperature sensors a fan, the works.

## Microcontroller

I'm using the Espressif ESP32 for this project, as it is a cheap part with wifi built-in. The code is written for the Espressif Internet-of-things Development Framework, which is composed of a FreeRTOS port, lwip, drivers and other components, written in C.

To build the code, follow the instructions

## Web app
The server side consists of three parts:

* An MQTT broker
* An MQTT client
* A Flask web app

The MQTT broker is where the ESP32 will publish its information to and receive commands from.

The MQTT client is a python background process, subscribing to the relevant topics and storing the information in a database, adding a timestamp for when the data was received.

The web application serves a website that can be used to monitor the data coming from the ESP32 and ultimately send commands to it.

## TODO

### uC
- [ ] Multiple sensors (labels?)
- [ ] Solenoid valves
- [ ] Start as AP, store known SSIDs etc (BootWifi or WifiManager).

### Webapp
- [ ] Clean up old records after some time
- [ ] Integrate MQTT client into webapp (flask-mqtt)
- [ ] Configuration from webapp

### Hardware
- [ ] Pump for pressure (auto shutoff, SingFlo?)
- [ ] Add flow meter

### Other
- [ ] Add Home Assistent IO integration
