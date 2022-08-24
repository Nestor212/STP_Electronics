/*
Implementation of Stepper Motors
*/

#include "STP_motors.h"
#include "STP_global.h"



motor::motor() {}

void motor::init ( int contPin, int dirP) {
    /*! @brief      Initializes the contents of the class
        @details    Sets pin definitions, and initializes the variables of the class.
        @param[in]  dirPin Defines which pin controls direction
        @param[in]  outPin Defines which pin controls output 
        @return     void */


    // Initialize the values

    dirPin = dirP;
    controlPin = contPin;
    direction = 1;

    pinMode(dirP,OUTPUT);
    pinMode(contPin,OUTPUT);
}

void motor::setSpeed(int value) {
    speed = 16000 - ((value - 1) * 3875);
}

void motor::setDir(int dir) {
    direction = dir;
}

int motor::getSpeed() {
    return speed;
}

int motor::getDir() {
    return direction;
 }