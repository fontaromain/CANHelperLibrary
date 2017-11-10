# CAN Helper Library

### Main goal
This project aims to offer a simple interface through CAN network.
It was written to help adding some new options to a Scion FRS car, like showing oil temperature, etc.

In order to work and be flexible it defines:
- a connector (bridge to a specific CAN shield or hardware)
- a basic frame model (Standard CAN Frame + ISO 15765-2)
- a basic OBD frame (that could send data, wait for a reply and extract the relevant values)
- some utilities methods
- some standard OBD frame (query water temperature, etc.) and some Toyota specific ones (lock doors, query oil temperature, etc.)

### Getting started
You could find the doxygen documentation under the _"Documentation"_ folder.
Please read it in order to understand how to use the library or ask me and I'll respond as soon as possible.
You could also use the _"ArduinoProjects"_ folder as a good start.

It has been tested on an Arduino UNO using Arduino IDE 1.8.2 and the CAN BUS Shield from http://wiki.seeed.cc/CAN-BUS_Shield_V1.2/ (which is provided in the connectors folders).

If you need to use a different CAN BUS shield than the one I used, simply create your specific connector using the provided ICANConnector interface.
You are welcome to send it to me so I can add it to the connectors list ;)

Have fun !