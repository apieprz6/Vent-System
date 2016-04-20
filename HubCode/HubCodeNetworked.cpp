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
extern "C" {
#include <wiringPi.h>
}

using namespace std;
RCSwitch mySwitch;

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

double* truncateSig(double sigs[], double value){//Signatures can only be in the single digit range.
    string modify = to_string(value);
    string signature = modify.substr(0,1);
    string data = modify.substr(1);
    int sig= stoi(signature);
    double tempData = stoi(data);
    //put sig and tempData into an array and return the array
}

int main(){
    double  fahren=0;
    const int RANGE = 1.5;
    int PIN = 2;
    double ventData[3]={0,0,0};
    if(wiringPiSetup() == -1) {
        printf("wiringPiSetup failed, exiting...");
        return 0;
    }
    mySwitch = RCSwitch();
    mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
    
    while(true){
        double recievedValue = getValue();
        double *tempArr = truncateSig(ventData[], recievedValue);
        ventData = *tempArr;
        recievedValue = recievedValue / 100;
        if((recievedValue > fahren+RANGE || recievedValue < fahren - RANGE) && recievedValue!=0){
            //if(recievedValue!=0){
            fahren = recievedValue;
            cout << fahren << endl;
            sendValue(fahren);
        }
    }
}