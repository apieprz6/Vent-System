//
//  HubCode.cpp
//  
//
//  Created by Alex Pieprzycki on 3/11/16.
//
//

#include "HubCode.h"
#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>


RCSwitch mySwitch;

int main(int argc, char *argv[]){
    double  fahren=0;
    const int RANGE = 1;
    while(true){
        double recievedValue = getValue() / 10;
        if(recievedValue > fahren+1 || recievedValue < fahren - 1){
            fahren = recievedValue;
            sendValue(fahren, argv[]);
        }
    }
    
}

void sendValue(int valueToSend, char argv[]){
    int PIN = 0;
    
    // Parse the firt parameter to this command as an integer
    int code = atoi(argv[1]);
    
    if (wiringPiSetup () == -1) return 1;
    printf("sending code[%i]\n", code);
    mySwitch = RCSwitch();
    mySwitch.enableTransmit(PIN);
    
    mySwitch.send(code, 24);

}

int getValue(){
    int PIN = 2;
    
    if(wiringPiSetup() == -1) {
        printf("wiringPiSetup failed, exiting...");
        return 0;
    }
    
    int pulseLength = 0;
    
    mySwitch = RCSwitch();
    if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
    mySwitch.enableReceive(PIN);  // Receiver on interrupt 0 => that is pin #2
    
        if (mySwitch.available()) {
            
            int value = mySwitch.getReceivedValue();
            
            if (value == 0) {
                printf("Unknown encoding\n");
            } else {
                
                printf("Received %i\n", mySwitch.getReceivedValue() );
                return mySwitch.getReceivedValue();
            }
            
            mySwitch.resetAvailable();
        
    }
}