
#include "STP_global.h"
#include "STP_serial.h"
#include "STP_motors.h"
#include "math.h"

#define TSOFFSET 0x0002
#define TSGAIN     0x0003
//A5 or A4 will be used for Load cell Analog Data
//A5 = SCL on CNC board
//A4 = SDA on CNC board

/*
Desired Behavior: 3 Stepper Motors  
                  Actuator Motor 
                    User input: single force command, start, stop, slew force @ rate X, ???
                    Outputs: Time in milliseconds, Echo of command, Stream of force datas, ???
                  HP Deployment Motor
                    User input: set velocity, extend, retract, start, stop, ???
                    Outputs: Time in milliseconds, Echo of command, current steps, end of travel reached
                  HP Length Control Motor 
                    User input: position command, set velocity, start, stop, ???
                    Outputs: Time in milliseconds, Echo of command, Stream of LVDT datas, current steps, ???
*/


/* 
deployment_control = X
force_control      = Y
length_control     = Z
*/

struct motor_config {
  int controlPin;
  int dirPin;
};

struct motor_config motor_cfg[] = {
  //controlPin, dirPin
  {2,5},
  {3,6},
  {4,7}
};


motor deployment_control, force_control, length_control;



double GetTemp(void)
{
  unsigned int wADC;
  float t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celsius.
  return (t);
}

void setup() {

  Serial.begin(9600);

  deployment_control.init(motor_cfg[0].controlPin, motor_cfg[0].dirPin);
  force_control.init(motor_cfg[1].controlPin, motor_cfg[1].dirPin);
  length_control.init(motor_cfg[2].controlPin, motor_cfg[2].dirPin);
}


void loop() {

  serial_read();
  parse_command();


  Serial.println("Constant Data StreamA: " + String(analogRead(A5)));
  delay(1000);
  Serial.println(GetTemp(),2);
  delay(1000);

  /*
  Serial.println("Rotatation begun");
  digitalWrite(force_control.dirPin, 1); // set direction, HIGH for clockwise, LOW for anticlockwise
  for (int i = 0 ; i < 400 ; i++) {
    digitalWrite(force_control.controlPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(force_control.controlPin, LOW); 
    delayMicroseconds(500);

    delay(500);
  }
  Serial.println("Rotatation complete");
  delay(2000); 

  */
}
