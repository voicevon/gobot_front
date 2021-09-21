#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

// #include "hardware.hpp"
#include "Robot/Commu/CommuBLE.h"
#include "gobot_house.h"
#include "MyLibs/MyFunctions.hpp" 
// #include "Robot/Axis/SingleAxisBase.hpp"

// static char LOG_TAG[]= "BLE-HOUSE";
GobotHouse* robot; 
RobotAction action;
// CommuBle ble= CommuBle();

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your bot,  Gobot-House. ");
    robot = &GobotHouse::getInstance();
    // ble.Init();
    //couple the components
    // robot->LinkCommuDevice(&ble);
    // robot->axis_alpha.LinkAcuator(&stepper_alpha);
    // robot->axis_beta.LinkAcuator(&stepper_beta);
    // robot->axis_alpha.LinkHomeTriger(&homeTriger_alpha);
    // robot->axis_beta.LinkHomeTriger(&homeTriger_beta);
    // robot->LinkActuatorController(&stepControl);

    robot->Setup(&action, 9);
    Serial.print("\nHouse setup is done..........");

    robot->HomeAllAxises();
    Serial.print("\nHouse Homing is done......");

}


void loop(){
    // ble.SpinOnce();
    robot->SpinOnce();

    // Notificate my status.
    // ble.UpdateCurrentPos(100);

    // if (robot->IsBusy())
    //     return;
    // if(ble.HasNewChatting()){
    //     // ble got new gcode
    //     Gcode gCode = Gcode(ble.ReadChatting());   //Risk for not releasing memory ?
    //     robot->RunGcode(&gCode);
    // }


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