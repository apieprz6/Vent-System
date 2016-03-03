#include <Servo.h>
#include <RCSwitch.h>
#include <stdlib.h>
#include <stdio.h>

RCSwitch rcSwitch = RCSwitch();
Servo servo;
const int TEMPSENSORPIN=A5;
const int SERVOPIN = 9;
const int TRANSMITPIN = 10;
const int RECIEVEPIN = 0; //Actually pin 2
int setMotorTo = 0;
int temp = 0; 
double fahren = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rcSwitch.enableTransmit(TRANSMITPIN);
  rcSwitch.enableReceive(RECIEVEPIN); //pin 2
  servo.attach(SERVOPIN);
  servo.write(0);
  delay(1000);
  servo.detach();
}

void loop() {
  if (rcSwitch.available()) { //Checks if radio has recieved transmissions
    temp = getMotorValue();
    if (setMotorTo != temp) { //If there has been a transmission, motor shifts to the recieved value
      setMotorTo = temp;
      servo.attach(SERVOPIN);
      servo.write(setMotorTo);
      delay(1000);
      servo.detach();
    }
  }
  int reading = analogRead(TEMPSENSORPIN);
  double temp =  Thermistor(reading);
  fahren = temp * 9 / 5 + 32;
}

int getMotorValue() {
  int value = rcSwitch.getReceivedValue();
  Serial.print("Received ");
  Serial.println(value);
  rcSwitch.resetAvailable();
  return value;
}

