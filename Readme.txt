This project aims to offer a simple interface through CAN network over Arduino (or other systems).
Use it at your own risks.

Please read the classes documentation in order to understand how to use the library or ask me and I'll respond ASAP.
You could use the Arduino projects as a good start :

ArduinoProjects : Base arduino projects folder
    | 
    |--- AutoLockDoorsFRS : Project that shows how to add "auto doors lock" to a Scion FRS / Toyota GT86 / Subaru BRZ
           |
           |--- AutoLockDoorsFRS.ino : Project code with the close / open logic 
                                       (closes if speed reaches 15kph, opens on engine stop)


If you need to use a different CAN BUS shield than the one I used, simply create your specific connector using the provided ICANConnector interface.
You are welcome to send it to me so I can add it to the connectors list ;)

For the record, I used the MCP CAN library that you could find in the subfolders, with Arduino IDE 1.8.2, a classic Arduino UNO and the CAN BUS Shield from http://wiki.seeed.cc/CAN-BUS_Shield_V1.2/

Have fun !