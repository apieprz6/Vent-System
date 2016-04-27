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
#include <sstream>
extern "C" {
#include <wiringPi.h>
}

using namespace std;
RCSwitch mySwitch;
const int SERVOOPEN = 180;
const int SERVOCLOSED = 0;
double vent1data = 0;
double vent1dataOld = 0;
double vent2data = 0;
double vent2dataOld = 0;
//const double DESIREDTEMP = 70;

string to_string (int num )
{
    ostringstream ss;
    ss << num;
    return ss.str();
}

int stoi ( string Text )
{
    istringstream ss(Text);
    int result;
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
    return -1;
}

void truncateSig(double value){//Signatures can only be in the single digit range.
    string modify = to_string(value);
    string signature = modify.substr(0,1);
    string data = modify.substr(1);
    cout<<data<<endl;
    int sig= stoi(signature);
    double tempData = (double)stoi(data)/100;
    cout<<"vent number: " << sig << " temp: " << tempData << endl;
    if(sig == 1){
        vent1data = tempData;
        cout << "Vent 1 = " << vent1data << endl;
    }
    else if(sig == 2){
        vent2data = tempData;
        cout << "Vent 2 = " << vent2data << endl;
    }
}

int concat (int sig, int servo){
    string signature = to_string(sig);
    string servopos = to_string(servo);
    string code = signature + servopos;
    return stoi(code);
}

void algorithm(double desiredTemp){
    double RANGE = 1;
    if(vent1data != vent1dataOld){
        vent1dataOld = vent1data;
        if(vent1data > desiredTemp + RANGE){
            sendValue(concat(1,SERVOOPEN));
        }
        else{
            sendValue(concat(1,SERVOCLOSED));
        }
    }
    if(vent2data != vent2dataOld){
        vent2dataOld = vent2data;
        if(vent2data > desiredTemp + RANGE){
            sendValue(concat(2,SERVOOPEN));
        }
        else{
            sendValue(concat(2,SERVOCLOSED));
        }
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
    cout << "Enter desired temperature: ";
    cin >> desiredTemp;
    cout << "Desired temp: " << desiredTemp << endl;
    
    while(true){
        int recievedValue = getValue();
        if(recievedValue != -1){
            truncateSig(recievedValue);
            //cout<<"Left TruncateSig"<<endl;
            algorithm(desiredTemp);
            //cout<<"Left algorithm"<<endl;
        }
    }
}