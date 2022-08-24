#include <stdint.h>
#include <avr/interrupt.h>
#include "STP_serial.h"
#include "STP_motors.h"
#include "STP_global.h"

/* 
deployment_control = X
force_control      = Y
length_control     = Z

Arduino A5 pin used to process Load Cell data 

*/



int speed = 5000; // default speed of actuator 
char command[20];
char data[10];
uint8_t char_counter;
char actuator;
int value = 0;
unsigned int dir = 0;
unsigned int steps = 0;

int i = 0;
char c;



void serial_read() {

  memset(data, 0, sizeof(data));  
  memset(command, 0, sizeof(command));

  char_counter = 0;

  while (Serial.available() > 0) {
    command[char_counter++] = Serial.read();
  }
  Serial.flush();
}




void parse_command() {

  extern motor deployment_control, force_control, length_control;

  actuator = command[0];

  if (command[1] == '=') {
    if(command[2] == '-'){ 
      char_counter = 3;
      while (command[char_counter] != NULL) {
      data[char_counter - 3] = command[char_counter];
      char_counter++;
      } 
      value = - (atoi(data));
      dir = 0;
    }
    else {
      char_counter = 2;
      while (command[char_counter] != NULL) {
      data[char_counter - 2] = command[char_counter];
      char_counter++;
      } 
      value = atoi(data);
      dir = 1;
    }
  }
  steps = abs(value);

  switch (actuator) {
    case 'F': 
      Serial.print("Setting force to: ");Serial.println(value);
      digitalWrite(force_control.dirPin, dir); // set direction, HIGH for clockwise, LOW for anticlockwise
      for(int x = 0; x < 20000; x++) { // loop for 200 steps
        digitalWrite(force_control.controlPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(force_control.controlPin, LOW); 
        delayMicroseconds(speed);
      }
      delay(1000); // delay for 1 second
      break;
    case 'X': 
      Serial.print("Stepping X: ");Serial.println(value);
      digitalWrite(deployment_control.dirPin, dir); // set direction, HIGH for clockwise, LOW for anticlockwise
      for(int x = 0; x < steps; x++) { // loop for 200 steps
        digitalWrite(deployment_control.controlPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(deployment_control.controlPin, LOW); 
        delayMicroseconds(speed);
      }
      delay(1000); // delay for 1 second
      break;
    case 'Y': 
      Serial.print("Stepping Y: ");Serial.println(value);    
      digitalWrite(force_control.dirPin, dir); // set direction, HIGH for clockwise, LOW for anticlockwise
      for(int x = 0; x < steps; x++) { // loop for 200 steps
        digitalWrite(force_control.controlPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(force_control.controlPin, LOW); 
        delayMicroseconds(speed);
      }
      delay(1000); // delay for 1 second
      break;
    case 'Z': 
      Serial.print("Stepping Z: ");Serial.println(value);
      digitalWrite(length_control.dirPin,  dir); // set direction, HIGH for clockwise, LOW for anticlockwise
      for(int x = 0; x < steps; x++) { // loop for 200 steps
        digitalWrite(length_control.controlPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(length_control.controlPin, LOW); 
        delayMicroseconds(speed);
      }
      delay(1000); // delay for 1 second     
      break;
    case 'D':
      Serial.println("Deploying...." );
      digitalWrite(deployment_control.dirPin, HIGH); //set direction, HIGH for clockwise
      for(int x = 0; x < 1000; x++) { // loop for 1000 steps
        digitalWrite(deployment_control.controlPin , HIGH);
        delayMicroseconds(speed);
        digitalWrite(deployment_control.controlPin, LOW); 
        delayMicroseconds(speed);
      }
      delay(1000); // delay for 1 second
      break;
    case 'R':
      Serial.println("Retracting....");
      digitalWrite(deployment_control.dirPin, LOW); //set direction, LOW for counterclockwise
      for(int x = 0; x < 1000; x++) { // loop for 1000 steps
        digitalWrite(deployment_control.controlPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(deployment_control.controlPin, LOW); 
        delayMicroseconds(speed);
      }
      delay(1000); // delay for 1 second
      break;
    case 'S':
      speed = 16000 - ((value - 1) * 3875);
      Serial.println("Speed level: " + String(value));
    default: 
      break; //Serial.println(command);   
  }

}

