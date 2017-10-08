# ChilIO

This is a little IoT project to monitor moisture levels and other parameters of (chili) plants, and automate the watering.

## Hardware

To measure the moisture level, some simple resistive moisture sensors are used. To enable/disable water flow, solenoid valves are used, which require a little circuit to drive from a microcontroller.

Later I would also like to add a flow meter, temperature sensors a fan, the works.

## ESP32 code

## Web app
The server side consists of three parts:

* An MQTT broker
* An MQTT client
* A Flask web app

The MQTT broker is where the ESP32 will publish its information to and receive commands from.

The MQTT client is a python background process, subscribing to the relevant topics and storing the information in a database

The web application serves a website that can be used to monitor the data coming from the ESP32 and ultimately send commands to it.

## TODO

### uC
- [ ] Multiple sensors (labels?)
- [ ] Solenoid valves
- [ ] Start as AP, store known SSIDs etc.

### Webapp
- [X] Make subscriber service
- [ ] Clean up old records after some time
- [ ] Endpoint for all values in last 24(?) hours (JSON)

