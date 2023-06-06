
#include "serial_port_sniffer_app.h"
#include <HardwareSerial.h>


SerialPortSniffer_App::SerialPortSniffer_App(){
    // this->__robot_row_id = row_id;
    Serial.print("\n[Info] SerialPortSniffer_App::SerialPortSniffer_App() is constructed");
}

void SerialPortSniffer_App::ExecuteCommand(CommandText* gcode_text){
    const char command_test_led[8] = {"app:led"};

    Logger::Info("SerialPortSniffer_App::ExecuteCommand()");
    // gcode_text->PrintFlat("SerialPortSniffer_App::ExecuteCommand()");

    // if (gcode_text->IsEqualTo(command_test_led)){
    //     for (int i=0; i<200; i++){
    //         __board->TestLeds(200);
    //     }
    // }
}





