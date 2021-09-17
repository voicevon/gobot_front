#include "all_devices.h"

#ifdef I_AM_GOBOT_HOUSE

// #include "board_gobot_house.h"
// #include "RobotJoint/SingleAxis.h"
#include "hardware.hpp"
#include "MyLibs/BleHelper.h"
#include "house.h"   
static char LOG_TAG[]= "BLE-HOUSE";

House* pHouse; 

BLECharacteristic* pCharRobotAction;
BLECharacteristic* pCharRobotState;
RobotAction action;

MyBleServerCallbacks* pMyBle;


// Board_GobotHouse board = Board_GobotHouse();
// DCMotor motor = DCMotor();
// SingleAxis axis = SingleAxis(0);
BleHelper bleHelper = BleHelper();

void setup(){
    // board.Flash_AllLeds(3,500,500);
    // board.flash();
    bleHelper.InitBle();

    pHouse = &House::getInstance();
    // pHouse->Test_home_sensor();
    pHouse->Setup(&action, 9);
    // pHouse->LinkStepper(board.stepper_alhpa, board.stepper_beta);
    Serial.print("\nHouse setup is done..........");

    pHouse->Home(HOUSE_ALPHA_AXIS);
    pHouse->Home(HOUSE_BETA_AXIS);
    Serial.print("\nHouse Homing is done......");

}


int8_t GetTrueBitIndex(uint8_t any){
  if (any % 2 ==1){
    // is doing a task.
    return -1;
  }
  for(int i=0; i<8; i++){
    if (((any >> i) & 0b00000001) == 1){
      // will start to do a task
      // action.bytes[0]++;   //??????????????
      return i;
    }
  }
  // No task at all
  return -2;
}

void loop(){
    if (! pMyBle->is_connected ){
        BLEDevice::startAdvertising();
        delay(5000);
    }

    if (action.bytes[0] <= 1){
        // Both head side and Esp side are idle. 
        uint8_t* pData  = pCharRobotAction->getData();
        for (int i=0; i<14; i++){
        action.bytes[i] = *(pData + i);
        // Serial.print(action.bytes[i]);
        // Serial.print(" ");
        }
        return;
    }
  
    int8_t true_bit  = GetTrueBitIndex(action.bytes[0]);

    uint8_t house_id;
    pHouse->SpinOnce();
    switch (true_bit){
        //All the below functions will modify action.bytes[0]
        case 6:
        house_id = action.House.from_start_house_id;
        pHouse->MoveStone_FromRoomToHead(house_id);
        break;
        case 7:
        house_id = action.House.from_start_house_id;
        pHouse->MoveStone_FromHeadToRoom(house_id);
        break;
        case HOUSE_ALPHA_AXIS:
        pHouse->Home(HOUSE_ALPHA_AXIS);
        break;
        case HOUSE_BETA_AXIS:
        pHouse->Home(HOUSE_BETA_AXIS);
        break;
        default:
        break;
    }
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