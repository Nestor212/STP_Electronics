# STP_Electronics
This project contains firmware for an Arduino Uno which serves as a controller for a stepper motor control system. 
Along with a CNC Shield board, the assembly is currently configured to run 3 stepper motors; x, y, z, and has capabilities for a 4th. 
Additionally, it can connect to a load cell through an amplifier circuit board that sends recived data to an arduino analog pin. 

## Connections
* J1 - Stepper Motor Connections
* J2 - Load Cell Connector

Please refer to schematic for pin configurations. (Can be found in this repository)


Follow link below for useful information regarding CNC board, including microstepping instructions. 

https://www.zyltech.com/arduino-cnc-shield-instructions/


A simple python command line interface has been written to command the three motors and display the following:

* Arduino Temperature
* Load Cell Data


The following commands are used to control the motors:
Commands:
speed=LEVEL : sets the speed of the actuator motors
LEVEL = number 0 (slowest) - 9 (fastest)
step(x/y/z)=STEPS : steps motor x, y, or z motor by entered STEPS 
STEPS : positive number for CW direction, 200 full steps = 1 rotation
negative number for CCW direction, 200 full steps = 1 rotation
Note: Only one command can be specified on the command-line.



## *Important* Power off Assembly and remove respective motor driver if a stepper motor is removed.

