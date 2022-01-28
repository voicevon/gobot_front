#include "CommuUart.h"
#include <HardwareSerial.h>

void CommuUart::OutputMessage(std::string message){
    Serial.print("[Info] CommuUart::OutputMessage() ...   ...   ...");
    Serial.println(message.c_str());
}
void CommuUart::WriteNotification(std::string notification){
    //For UART, will do same thing to OutputMessage()
    //Reason:  UART has ONLY ONE channel to write out.
    //         while WriteNotification should output message to a one-way individual channel.
    this->OutputMessage(notification);
}

// void CommuUart::OutputMessage(char* message){
//     Serial.print("\n...   ...   ...");
//     Serial.print(message);
// }
void CommuUart::SpinOnce(){

}
bool CommuUart::HasNewChatting(){
    return false;
} 

char* CommuUart::ReadChatting(){
    return this->pRxBuffer;
}

void CommuUart::Init(){
    for(int i=0; i<20; i++){
        RxBuffer[i] = 65;
    }
    RxBuffer[20] = 0;
    pRxBuffer = &RxBuffer[0];
}
