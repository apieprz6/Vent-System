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
    mySwitch = RCSwitch();
    mySwitch.enableTransmit(PIN);
    
    mySwitch.send(code, 24);
    mySwitch.send(code, 24);
    
}

int getValue(){
    cout << "IN GETVALUE " << endl;
    int PIN = 2;
    
    //    if(wiringPiSetup() == -1) {
    //        printf("wiringPiSetup failed, exiting...");
    //        return 0;
    //    }
    while(1){
        //cout << mySwitch.available();
        if (mySwitch.available()) {
            int value = mySwitch.getReceivedValue();
            if (value == 0) {
                printf("Unknown encoding\n");
            }
            else {
                
                printf("Received %i\n", mySwitch.getReceivedValue() );
                return mySwitch.getReceivedValue();
            }
            
            //mySwitch.resetAvailable();
            
        }
        //mySwitch.resetAvailable();
    }
    
    return 0;
}

int main(){
    double  fahren=0;
    const int RANGE = 1;
    int PIN = 2;
    if(wiringPiSetup() == -1) {
        printf("wiringPiSetup failed, exiting...");
        return 0;
    }
    mySwitch = RCSwitch();
    mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
    while(true){
        double recievedValue = getValue();
        mySwitch.resetAvailable();
        cout << " out of getValue() ";
        recievedValue = recievedValue / 100;
        //if((recievedValue > fahren+RANGE || recievedValue < fahren - RANGE) && recievedValue!=0){
        if(recievedValue!=0){
            fahren = recievedValue;
            cout << fahren << endl;
            sendValue(fahren);
        }
    }
}