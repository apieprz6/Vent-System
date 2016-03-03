#include <Servo.h>
#include <RCSwitch.h>
#include <stdlib.h>
#include <stdio.h>

RCSwitch rcSwitch = RCSwitch();
Servo servo;
const int SERVOPIN=9;
const int TRANSMITPIN=10;
const int RECIEVEPIN=0;//Actually pin 2
int setMotorTo=0;
int temp=0;

void setup() {
  // put your setup code here, to run once:
  rcSwitch.enableTransmit(TRANSMITPIN);
  rcSwitch.enableReceive(RECIEVEPIN); //pin 2
  servo.attach(SERVOPIN);
  servo.write(0);
  servo.detach();
}

void loop() {
  if(rcSwitch.available()){ //Checks if radio has recieved transmissions
    temp = getMotorValue();
  }
  if(setMotorTo != temp){ //If there has been a transmission, motor shifts to the recieved value
    setMotorTo = temp;
    servo.attach(SERVOPIN);
    servo.write(0);
    servo.detach();
  }
}

int getMotorValue(){
  int value = rcSwitch.getReceivedValue();
  return value;
}

