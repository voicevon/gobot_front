#include "CommuUart.h"
#include <HardwareSerial.h>

void CommuUart::OutputMessage(std::string message){
    Serial.print("\n...   ...   ...");
    Serial.print(message.c_str());
}

void CommuUart::OutputMessage(char* message){
    Serial.print("\n...   ...   ...");
    Serial.print(message);
}
void CommuUart::SpinOnce(){

}
bool CommuUart::HasNewChatting(){

} 

char* CommuUart::ReadChatting(){
    return this->pRxBuffer;
}

void CommuUart::WriteNotification(const char* notification){

}