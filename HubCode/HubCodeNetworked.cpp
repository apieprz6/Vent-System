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
#include <vector>
#include <fstream>
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

int* loadArray(int length){
    int sigs[5] = {};
    ifstream in("Signatures.txt");
    if(fout.is_open()){
        //file opened successfully so we are here
        cout << "File Opened successfully!!!. Writing data from array to file" << endl;
        for(int i = 0; i<length; i++){
            Signatures >> sigs[i]; //writing ith character of array in the file
        }
        cout << sigs << endl;
    }
    else //file could not be opened
    {
        cout << "File could not be opened." << endl;
    }
    return sigs;
}

int main(){
    double  fahren=0;
    const int RANGE = 1;
    const int RADIOCODE = 99;
    int PIN = 2;
    //vector<int> signatures;
    int *signatures= loadArray(5);
    if(wiringPiSetup() == -1) {
        printf("wiringPiSetup failed, exiting...");
        return 0;
    }
    mySwitch = RCSwitch();
    mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
    
    while(true){
        double recievedValue = getValue();
        if(recievedValue == 99){
            //signatures.push_back(signatures.back()+1);
            //cout << signatures << endl;
            //string sigToSend = "99" + to_string(signatures.back());
            //int codeToSend = stoi(sigToSend);
            //sendValue(codeToSend);
        }
        recievedValue = recievedValue / 100;
        if((recievedValue > fahren+RANGE || recievedValue < fahren - RANGE) && recievedValue!=0){
        //if(recievedValue!=0){
            fahren = recievedValue;
            cout << fahren << endl;
            sendValue(fahren);
        }
    }
}
