#include "CommuSingleAxisRobotUart.h"
#include <HardwareSerial.h>

void CommuSingleAxisRobotUart::OutputMessage(std::string message){
    Serial.print("\n...   ...   ...");
    Serial.print(message.c_str());
}

void CommuSingleAxisRobotUart::SpinOnce(){

}
bool CommuSingleAxisRobotUart::HasNewChatting(){

} 

char* CommuSingleAxisRobotUart::ReadChatting(){
    return this->pRxBuffer;
}

void CommuSingleAxisRobotUart::WriteNotification(const char* notification){

}