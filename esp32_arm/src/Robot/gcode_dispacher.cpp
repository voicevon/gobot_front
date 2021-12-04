#include "gcode_dispacher.h"
#include "HardwareSerial.h"

GcodeDispacher::GcodeDispacher(){
}

void GcodeDispacher::LinkRobot(RobotBase* mybot){
    this->_myBot = mybot;
    this->head = 0;
    this->tail = 0;
}

void GcodeDispacher::SpinOnce(){
    if (_myBot->State == RobotBase::IDLE){
        // Serial.println("Dispacher, Got robot idle.");
        if(this->head != this->head){
            //Run next gcode
            Serial.println("Start to run next gcode");
            this->_myBot->RunGcode(this->_buffer[tail]);
            this->tail++;
            if (this->tail == 5) this->tail = 0;
        }
    }
    _myBot->SpinOnce();
}

bool GcodeDispacher::AppendGcode(Gcode* gcode){
    int pre_head = this->head;
    pre_head++;
    if (pre_head == 5) pre_head =0;
    if (pre_head == this->tail){
        // Buffer is full
        return false;
    }

    this->head = pre_head;
    this->_buffer[this->head] = gcode;
    Serial.print("Gcode added  ");
    Serial.println(gcode->get_command());
    return true;
}