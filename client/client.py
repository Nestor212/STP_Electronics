from asyncio.windows_events import NULL

import serial
import time
import sys
import threading
import logging

#Application Constants

APP_NAME = "STP Electronics"
APP_VERSION = "1.0"
APP_DESCRIPTION = "STP Electronics unser interface"
APP_URL = "http://localhost:8000/"
option_no_GUI = True
SerialObj = NULL
close_thread = False

'''
Desired Behavior: 3 Stepper Motors  
                  Actuator Motor 
                    User input: single force command, start, stop, slew force @ rate X, ???
                    Outputs: Time in milliseconds, Echo of command, Stream of force values, ???
                  HP Deployment Motor
                    User input: set velocity, extend, retract, start, stop, ???
                    Outputs: Time in milliseconds, Echo of command, current steps, end of travel reached
                  HP Length Control Motor 
                    User input: position command, set velocity, start, stop, ???
                    Outputs: Time in milliseconds, Echo of command, Stream of LVDT values, current steps, ???
'''

# Set up logging
# create logger
main_logger = logging.getLogger('main_logger')
logging_level = logging.DEBUG
main_logger.setLevel(logging_level)
# create console handler and set level to debug
ch = logging.FileHandler('STP_Electronics_Data', mode='a', encoding=None, delay=False, errors=None)
ch.setLevel(logging_level)
# create formatter
formatter = logging.Formatter('%(asctime)s   %(name)-12s   %(levelname)-8s   %(message)s')
# add formatter to ch
ch.setFormatter(formatter)
# add ch to logger
main_logger.addHandler(ch)

# Logging levels:
# CRITICAL  50
# ERROR		40
# WARNING	30
# INFO      20
# DEBUG		10
# NOTSET	0







def connect_to_Arduino(SerialPortNum):
    global SerialObj

    try:
        SerialObj = serial.Serial(SerialPortNum, 9600)
    except serial.SerialException as var: 
        print(f'An exception occured, exception details: {var}')
        main_logger.error(f'An exception occured, exception details: {var}')
    else: 
        SerialObj.bytesize = 8     # Number of data bits = 8
        SerialObj.parity   ='N'    # No parity
        SerialObj.stopbits = 1     # Number of Stop bits = 1
        print(f'Connected to Arduino on serial port: {SerialPortNum}')
        main_logger.info(f'Connected to Arduino on serial port: {SerialPortNum}')
        time.sleep(3)

def show_usage():
    print(f'STP Electronics User Interface v{APP_VERSION}')
    print('Commands: ')
    print(' ')

def client_loop():
    global SerialObj
    while not close_thread:
        time.sleep(0.1)
        data1 = SerialObj.readline()
        print(data1)

if option_no_GUI:

    print(f'STP Electronics User Interface v{APP_VERSION}')

    SerialPortNum = input('\nEnter Port Number: ')
    connect_to_Arduino(SerialPortNum)

    close_thread = False
    threading.Thread(target = client_loop).start()

    while True:
        try: 
            command = input('Enter command (? for help, ctrl-c to quit): \n').lower().split('=')
        except EOFError:
            close_thread = True
            sys.exit()
        if len(command) == 0:
            continue
        elif command == 'quit' or command == 'exit':
            close_thread = True
            sys.exit()
        elif command[0] == 'deploy':
            SerialObj.write(b'D')
        elif command[0] == 'retract':
            SerialObj.write(b'R')
        elif command[0] == 'force':
            command = f'F={command[1]}'
            SerialObj.write(command.encode())
        elif command[0] == 'stepx':
            command = f'X={command[1]}'
            SerialObj.write(command.encode())
        elif command[0] == 'stepy':
            command = f'Y={command[1]}'
            SerialObj.write(command.encode())
        elif command[0] == 'stepz':
            command = f'Z={command[1]}'
            SerialObj.write(command.encode())
        elif command[0] =='speed':
            command[1] = int(command[1])
            if ((command[1] < 1 ) or (command[1] > 5) ):
                print('Invalid entry, please enter a speed level between 1 (slowest) - 5 (fastest). ')
            else:
                command = f'S={command[1]}'
                SerialObj.write(command.encode())
        elif command[0] == 'help' or command[0] == 'h' or command[0] == '?':
            print( f'STP Electronics User Interface v{APP_VERSION}')
            print( f'Commands:')
            print( f'         speed=LEVEL : sets the speed of the actuator motors')
            print( f'               LEVEL = number 1(slowest) - 5(fastest)')
            print( f'         step(x/y/z)=STEPS : steps motor x, y, or z motor by entered STEPS ')
            print( f'               STEPS : positive number for CW direction, 200 full steps = 1 rotation')
            print( f'                       negative number for CCW direction, 200 full steps = 1 rotation')
            print( f'   ')
            print( f'   ')
            print( f'   ')
            print( f'   ')
            print( f'Note: Only one command can be specified on the command-line.')
        else:
            command1 = command[0]
            SerialObj.write(command1.encode())
            



