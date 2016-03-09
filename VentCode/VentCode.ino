/*  This class is for the arduino embedded into the vent/register.
 *  All that this code handles is recieving a value between 0-180,
 *  and adjusting a servo accordingly. The range of values may be
 *  adjusted for when we have a physical vent and actual values 
 *  differ from 0-180.
 */
 
/*   EDITS:
 *   1.) Remove print lines when code is finalized
 */
 
#include <RCSwitch.h>
#include <Servo.h>
#include <stdlib.h>
#include <stdio.h>

RCSwitch rcSwitch = RCSwitch();
Servo servo;
const int RECIEVEPIN =0;
const int SERVOPIN=9;
int setMotorTo=0;

void setup() {
  Serial.begin(9600);
  rcSwitch.enableReceive(RECIEVEPIN);//pin 2
}

void loop() {
  
  if(rcSwitch.available()){
    Serial.println("Entered .available");
    int temp = getMotorValue();//IF THERE IS A PROBLEM WITH CODE, MAKE THIS A GLOBAL VARIABLE
    if (setMotorTo != temp) { //If there has been a transmission, motor shifts to the recieved value
      setMotorTo = temp;
      servo.attach(SERVOPIN);
      servo.write(setMotorTo);
      delay(1000);
      servo.detach();
    }
  }

}

int getMotorValue(){
  int value = rcSwitch.getReceivedValue();
  Serial.print("Received ");
  Serial.println(value);
  rcSwitch.resetAvailable();
  return value;
}

