//
//  HubCode.cpp
//
//
//  Created by Alex Pieprzycki on 3/11/16.
//
//

//#include "HubCode.h"
#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
extern "C" {
#include <wiringPi.h>
}

using namespace std;
RCSwitch mySwitch;
const int SERVOOPEN = 180;
const int SERVOCLOSED = 0;
double vent1data = 0;
double vent2data = 0;
//const double DESIREDTEMP = 70;

string to_string (int Number )
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

double stoi ( string Text )
{
    istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

void sendValue(int valueToSend){
    int PIN = 0;
    
    // Parse the firt parameter to this command as an integer
    int code = valueToSend;
    
    printf("sending code[%i]\n", code);
    cout<<endl;
    mySwitch = RCSwitch();
    mySwitch.enableTransmit(PIN);
    
    mySwitch.send(code, 24);
    mySwitch.send(code, 24);
    
}

int getValue(){
    int PIN = 2;
    if (mySwitch.available()) {
        int value = mySwitch.getReceivedValue();
        if (value == 0) {
            printf("Unknown encoding\n");
        }
        else {
            printf("Received %i\n", mySwitch.getReceivedValue() );
            int temp = mySwitch.getReceivedValue();
            mySwitch.resetAvailable();
            return temp;
        }
        mySwitch.resetAvailable();
    }
    return 0;
}

void truncateSig(double value){//Signatures can only be in the single digit range.
    string modify = to_string(value);
    string signature = modify.substr(0,1);
    string data = modify.substr(1);
    cout<<data<<endl;
    int sig= stoi(signature);
    double tempData = std::stoi(data)/100;
    cout<<"vent number: " << sig << " temp: " << tempData << endl;
    if(sig == 1){
        vent1data = tempData;
        cout << vent1data << endl;
    }
    else{
        vent2data = tempData;
        cout << vent2data << endl;
    }
}

int concat (int sig, int servo){
    string signature = std::to_string(sig);
    string servopos = std::to_string(servo);
    string code = signature + servopos;
    return std::stoi(code);
}

void algorithm(double desiredTemp){
    double RANGE = 1;
    if(vent1data > desiredTemp + RANGE){
        sendValue(concat(1,SERVOOPEN));
    }
    else{
        sendValue(concat(1,SERVOCLOSED));
    }
    if(vent2data > desiredTemp + RANGE){
        sendValue(concat(2,SERVOOPEN));
    }
    else{
        sendValue(concat(2,SERVOCLOSED));
    }
}

int main(){
    double desiredTemp = 0;
    //double  fahren=0;
    //const int RANGE = 1.5;
    int PIN = 2;
    if(wiringPiSetup() == -1) {
        printf("wiringPiSetup failed, exiting...");
        return 0;
    }
    mySwitch = RCSwitch();
    mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
    cout << "Enter desired temperature";
    cin >> desiredTemp;
    cout << endl;
    
    while(true){
        //double recievedValue = getValue();
        //double *tempArr = truncateSig(ventData[], recievedValue);
        //ventData = *tempArr;
        truncateSig(getValue());
        algorithm(desiredTemp);
        //recievedValue = recievedValue / 100;
        //if((recievedValue > fahren+RANGE || recievedValue < fahren - RANGE) && recievedValue!=0){
            //if(recievedValue!=0){
            //fahren = recievedValue;
            //cout << fahren << endl;
            //sendValue(fahren);
        //}
    }
}