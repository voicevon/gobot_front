#include "all_devices.h"
#ifdef I_AM_UINIT_TEST
#include <SoftwareSerial.h>

#define MYPORT_TX 12
#define MYPORT_RX 23

SoftwareSerial myPort;



void setup(){
    Serial.begin(115200);
    Serial.println("Hi, ...");
    myPort.begin(115200, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX);
}

int count=0;
void loop(){
    char x;
    char* p=&x;
    if (myPort.available() > 0){
        if (count ==0){
            myPort.read(p,1);
            Serial.print(x);
        }
    }
    else
    {
        Serial.print(" ");
        count++;
        if (count > 100000);
            count =0;
    }
}


#endif
