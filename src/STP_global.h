#include <Arduino.h>
#include <stdio.h>
#include <string.h>


const int force_control_pin = 2;   //Actuator Motor
const int force_dir_pin = 5;
const int length_control_pin = 3;  //Hardpoint Length Control Motor
const int length_dir_pin = 6;
const int deploy_control_pin = 4;  //Hardpoint Deployment Motor 
const int deploy_dir_pin = 7;

// Define homing/hard limit switch input pins and limit interrupt vectors. 
// NOTE: All limit bit pins must be on the same port, but not on a port with other input pins (CONTROL).
#define LIMIT_DDR        DDRB
#define LIMIT_PIN        PINB
#define LIMIT_PORT       PORTB
#define X_LIMIT_BIT      1  // Uno Digital Pin 9
#define Y_LIMIT_BIT      2  // Uno Digital Pin 10
#ifdef VARIABLE_SPINDLE // Z Limit pin and spindle enabled swapped to access hardware PWM on Pin 11.  
  #define Z_LIMIT_BIT	   4 // Uno Digital Pin 12
#else
  #define Z_LIMIT_BIT    3  // Uno Digital Pin 11
#endif

#define SERIAL_NO_DATA 0xff


#define CMD_STATUS_REPORT '?'
#define CMD_FEED_HOLD '!'
#define CMD_CYCLE_START '~'
#define CMD_RESET 0x18 // ctrl-x

#define BAUD_RATE 9600
