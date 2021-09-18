#include "all_devices.h"

#ifdef I_AM_GOBOT_HOUSE

#include "hardware.hpp"
#include "Robot/RobotBLE.h"
#include "house.h" 

static char LOG_TAG[]= "BLE-HOUSE";

House* robot; 

RobotAction action;

RobotBle ble= RobotBle();

void setup(){
    robot = &House::getInstance();
    ble.Init();
    setup_hardware();
    //couple the components
    robot->axis_alpha->LinkAcuator(&stepper_alpha);
    robot->axis_beta->LinkAcuator(&stepper_beta);
    robot->axis_alpha->LinkHomeTriger(&homeTriger_alpha);
    robot->axis_beta->LinkHomeTriger(&homeTriger_beta);

    robot->Setup(&action, 9);
    Serial.print("\nHouse setup is done..........");

    robot->HomeAllAxises();
    Serial.print("\nHouse Homing is done......");

}


void loop(){
    ble.SpinOnce();
    robot->SpinOnce();

    // Notificate my status.
    // ble.UpdateCurrentPos(100);

    if (robot->IsBusy())
        return;
    if(ble.HasNewChatting()){
        // ble got new gcode
        Gcode gCode = Gcode(ble.ReadChatting());   //Risk for not releasing memory ?
        robot->RunGcode(&gCode);
    }


    // if (action.bytes[0] <= 1){
    //     // Both head side and Esp side are idle. 
    //     // uint8_t* pData  = pCharRobotAction->getData();
    //     uint8_t* pData  = ble.GetBuffer();
    //     for (int i=0; i<14; i++){
    //     action.bytes[i] = *(pData + i);
    //     // Serial.print(action.bytes[i]);
    //     // Serial.print(" ");
    //     }
    //     return;
    // }
  
    // int8_t true_bit  = GetTrueBitIndex(action.bytes[0]);
    // int8_t true_bit  = 1;

    // uint8_t house_id;
    // house->SpinOnce();
    // switch (true_bit){
    //     //All the below functions will modify action.bytes[0]
    //     case 6:
    //     house_id = action.House.from_start_house_id;
    //     house->MoveStone_FromRoomToHead(house_id);
    //     break;
    //     case 7:
    //     house_id = action.House.from_start_house_id;
    //     house->MoveStone_FromHeadToRoom(house_id);
    //     break;
    //     case HOUSE_ALPHA_AXIS:
    //     house->Home(HOUSE_ALPHA_AXIS);
    //     break;
    //     case HOUSE_BETA_AXIS:
    //     house->Home(HOUSE_BETA_AXIS);
    //     break;
    //     default:
    //     break;
    // }
  // pCharacteristic->setValue(action.bytes,13);

  // if (action.bytes[0] >=2){
  //   Serial.print("hhhhhhhhhhhhhhhhhhhhhhhhhhh    ");
  //   Serial.println(action.bytes[0]);
  // }else{
  //   Serial.print(">>>>>>>>>>>>>>>>>>>>>  ");
  //   Serial.print(action.bytes[0]);
  // }
  delay(500);
}


#endif