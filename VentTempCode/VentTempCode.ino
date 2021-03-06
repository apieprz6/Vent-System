/*   This code is in the event we only have the vent 
 *   finished and no external temperature reader.
 *   Handles both reading and sending temperature data, 
 *   as well as recieving servo commands from the central 
 *   hub and adjusting a servo. Other classes VentCode 
 *   and TempCode, handle these tasks seperatly.
 */

#include <Servo.h>
#include <RCSwitch.h>
#include <stdlib.h>
#include <stdio.h>

#define aref_voltage 3.3

RCSwitch rcSwitch = RCSwitch();
Servo servo;
const int TEMPSENSORPIN=0;//analog pin 0
const int TEMPPIN=1;//analog pin 1 for the temperature sensor
const int SERVOPIN = 9;
const int TRANSMITPIN = 10;
const int RECIEVEPIN = 0; //Actually pin 2
const unsigned long long int INTERVAL = 5000;//variable used to establish refresh time for sending temp data, currently set to 2 seconds
unsigned long long int previousMillis = 0;
int setMotorTo = 0;
double fahren = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
  analogReference(EXTERNAL);
  rcSwitch.enableTransmit(TRANSMITPIN);
  rcSwitch.enableReceive(RECIEVEPIN); //pin 2
  servo.attach(SERVOPIN);
  servo.write(0);
  delay(1000);
  servo.detach();
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
  if((currentMillis - previousMillis) >= INTERVAL){
    previousMillis = currentMillis;
//    int reading = analogRead(TEMPSENSORPIN);
//    double celsius =  Thermistor(reading);
//    fahren = celsius * 9 / 5 + 32; 
    Serial.print("Temp = ");
    Serial.print(tmp36());
    Serial.println("F");
    double tempToSend = tmp36() *100;
    Serial.print("Sending ");
    Serial.println((int)tempToSend);
    rcSwitch.send((int)tempToSend,24);
    rcSwitch.send((int)tempToSend,24);
  }
}

int getMotorValue() {
  int value = rcSwitch.getReceivedValue();
  Serial.print("RECIEVED ");
  Serial.println(value);
  rcSwitch.resetAvailable();
  return value;
}

//double Thermistor(int RawADC) {
//  double Temp;
//  Temp = log(((10240000 / RawADC) - 10000));
//  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
//  Temp = Temp - 273.15;
//  return Temp;
//}

double tmp36(){
  int tempReading;
  tempReading = analogRead(TEMPPIN);
  double voltage = tempReading * aref_voltage;
  voltage /= 1024.0;
  double temperatureC = (voltage - 0.5) * 100 ;
  double temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  return temperatureF;
}
