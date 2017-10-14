This project aims to offer a simple interface through CAN network over Arduino (or other systems).
Use it at your own risks.

Here is what you need to know about the library folders and files:

CANHelperLibrary : Base library folder
    |
    |--- CGlobals.h : Common defines used by the library
    | 
    |--- Frame : Folder that contains frames classes and definitions
    |      |
    |      |--- AFrame.h : Abstract frame class
    |      |
    |      |--- Extended : Folder that contains Extended frames classes and definitions (TODO)
    |      |
    |      |--- Standard : Folder that contains Standard frames classes and definitions
    |            |
    |            |--- CStdDataFrame.h : Base standard frame definition 
    |            |
    |            |--- CSendStdDataFrame.h : A standard frame designed to be sent over CAN
    |            |
    |            |--- CSendStdDataFrameEx.h : A standard frame designed to be sent over CAN 
    |            |                              with some parsing ability (for replies)
    |            |
    |            |--- CStdDataFrameReader.h : A standard frame only designed to be read from our CAN bus
    |            
    |--- Connector : Folder that contains our CAN connectors and that uses our specific frames classes
           |
           |--- ICANConnector.h : CAN connector interface definition
           |
           |--- MCPCAN
                   |
                   |--- CMCPCANConnector.h : Specific MCP CAN library connector implementation 
                               (CAN BUS shield http://wiki.seeed.cc/CAN-BUS_Shield_V1.2/)

ArduinoProjects : Base arduino projects folder
    | 
    |--- AutoLockDoorsFRS : Project that you could use as a sample start. 
           |                It shows how to add "auto doors lock" to a Scion FRS / Toyota GT86 / Subare BRZ
           |
           |--- AutoLockDoorsFRS.ino : Project code with the close / open logic 
                                       (closes if speed reaches 18kph, opens on engine stop)


If you need to use a different CAN BUS shield than the one I used, simply create your specific connector using the provided ICANConnector interface.
You are welcome to send it to me so I can add it to the connectors list ;)

For the record, I used the MCP CAN library that you could find in the subfolders, with Arduino IDE 1.8.2, a classic Arduino UNO and the CAN BUS Shield from http://wiki.seeed.cc/CAN-BUS_Shield_V1.2/

Have fun !