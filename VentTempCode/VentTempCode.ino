/*   This code is in the event we only have the vent
     finished and no external temperature reader.
     Handles both reading and sending temperature data,
     as well as recieving servo commands from the central
     hub and adjusting a servo. Other classes VentCode
     and TempCode, handle these tasks seperatly.
*/

#include <EEPROM.h>
#include <Servo.h>
#include <RCSwitch.h>
#include <stdlib.h>
#include <stdio.h>


RCSwitch rcSwitch = RCSwitch();
Servo servo;
const int TEMPSENSORPIN = 0; //analog pin 0
const int SERVOPIN = 9;
const int TRANSMITPIN = 10;
const int RECIEVEPIN = 0; //Actually pin 2
const int RADIOCODE = 99;
const unsigned long long int INTERVAL = 5000;//variable used to establish refresh time for sending temp data, currently set to 2 seconds
unsigned long long int previousMillis = 0;
int setMotorTo = 0;
double fahren = 0;
int signature = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rcSwitch.enableTransmit(TRANSMITPIN);
  rcSwitch.enableReceive(RECIEVEPIN); //pin 2
  servo.attach(SERVOPIN);
  servo.write(0);
  delay(1000);
  servo.detach();
  if (EEPROM.read(0) == 0) {//WILL BE STUCK IN INFINITE LOOP UNTIL RECIEVES SIGNATURE MESSAGE
    getSignature();
    EEPROM.write(0,signature);
  }
  else {
    signature = EEPROM.read(0);
  }
}

void loop() {
  if (rcSwitch.available()) { //Checks if radio has recieved transmissions
    int temp = getMotorValue();//IF THERE IS A PROBLEM WITH CODE, MAKE THIS A GLOBAL VARIABLE
    if (setMotorTo != temp) { //If there has been a transmission, motor shifts to the recieved value
      setMotorTo = temp;
      servo.attach(SERVOPIN);
      servo.write(setMotorTo);
      delay(1000);
      servo.detach();
    }
  }
  unsigned long long int currentMillis = millis();
  if ((currentMillis - previousMillis) >= INTERVAL) {
    previousMillis = currentMillis;
    int reading = analogRead(TEMPSENSORPIN);
    double celsius =  Thermistor(reading);
    fahren = celsius * 9 / 5 + 32;
    Serial.print("Temp = ");
    Serial.print(fahren);
    Serial.println("F");
    double tempToSend = fahren * 100;
    sendValue((int)tempToSend);
    }
}

void getSignature() {
  rcSwitch.send(RADIOCODE,24);//lets hub know what kind of device, and that it wants a signature
  while (signature == 0) { //infinite loop till vent gets signature wich starts with 99
    if (rcSwitch.available()) {
      int recieved = rcSwitch.getReceivedValue();
      String recievedString = String(recieved);
      if(recievedString.startsWith(String(RADIOCODE))){
        recievedString.remove(0,String(RADIOCODE).length());
        signature = recievedString.toInt();
      }
    }
    rcSwitch.resetAvailable();
  }
}

void sendValue(int value){
  String sig = String(signature);
  String valString = String(value);
  String tempString = sig.concat(valString);
  rcSwitch.send(tempString.toInt(),24);
}

int getMotorValue() {
  int value = rcSwitch.getReceivedValue();
  String valueString = String(value);
  if(valueString.startsWith(String(RADIOCODE))){
    return setMotorTo;
  }
  else if(valueString.startsWith(String(signature))){
    valueString.remove(0,String(signature).length());
    value = valueString.toInt();
  }
  Serial.print("Received ");
  Serial.println(value);
  rcSwitch.resetAvailable();
  return value;
}

double Thermistor(int RawADC) {
  double Temp;
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;
  return Temp;
}
