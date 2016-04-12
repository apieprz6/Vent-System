/*   For External temperature sensor. Handles
 *   sensing temperature using an analog temperature
 *   sensor and radioing the information back to the 
 *   central hub (raspberry pi in our case).
 */

#include <RCSwitch.h>
#include <stdlib.h>
#include <stdio.h>

#define aref_voltage 3.3

RCSwitch rcSwitch = RCSwitch();
const int THERMISTORPIN = 0;//Analog pin 0
const int TEMPPIN=1;
const int TRANSMITPIN = 10;
const unsigned long long int INTERVAL = 5000;//This is the refresh timer, it determines 
//how often temp data gets sent to the pi. Currently set to 5 seconds, 
//for actual implementation it should be set to more like every 5 min.
unsigned long long int previousMillis = 0;//This might be a problem when time starts to get massive

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  rcSwitch.enableTransmit(TRANSMITPIN);
}

void loop() {
  unsigned long long int currentMillis = millis();
  if((currentMillis - previousMillis) >= INTERVAL){
    previousMillis = currentMillis;
//    int reading = analogRead(THERMISTORPIN);
//    double celsius = Thermistor(reading);
//    double fahren = celsius * 9 / 5 + 32;
//    Serial.print("Thermistor Temp = ");
//    Serial.print(fahren);
//    Serial.println("F");
    Serial.print("temp36 temp = ");
    Serial.print(tmp36());
    Serial.println("F");
    double tempToSend = tmp36() * 100;
    rcSwitch.send((int)tempToSend,24);//change to tmp36 plz
  }
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

