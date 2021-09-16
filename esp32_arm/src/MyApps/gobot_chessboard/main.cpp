#include "all_devices.h"

#ifdef I_AM_GOBOT_CHESSBOARD

#include "board_gobot_chessboard.h"
#include "MyLibs/BleHelper.h"
#include "gobot_chessboard.h"   
static char LOG_TAG[]= "BLE-CHESSBOARD";

GobotChessboard* pArm; 

BLECharacteristic* pCharRobotAction;
BLECharacteristic* pCharRobotState;
RobotAction action;

MyBleServerCallbacks* pMyBle;


Board_GobotChessboard board = Board_GobotChessboard();
// DCMotor motor = DCMotor();
// SingleAxis axis = SingleAxis(0);
BleHelper bleHelper = BleHelper();

void setup(){
    board.Flash_AllLeds(3,500,500);
    bleHelper.InitBle();

    pArm = &GobotChessboard::getInstance();
    // pArm->Test_home_sensor();
    pArm->Setup(&action);
    // pArm->LinkStepper(board.stepper_alhpa, board.stepper_beta);
    Serial.print("\nArm setup is done..........");

    pArm->Home(6);
    pArm->Home(5);
    Serial.print("\nArm Homing is done......");

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

    uint8_t Arm_id;
    pArm->SpinOnce();
    switch (true_bit){
        //All the below functions will modify action.bytes[0]
        case 6:
          // Arm_id = action.Arm.from_start_Arm_id;
          // pArm->MoveStone_FromRoomToHead(Arm_id);
        break;
        case 7:
          // Arm_id = action.Arm.from_start_Arm_id;
          // pArm->MoveStone_FromHeadToRoom(Arm_id);
        break;
          // case Arm_ALPHA_AXIS:
          // pArm->Home(Arm_ALPHA_AXIS);
        break;
          // case Arm_BETA_AXIS:
          // pArm->Home(Arm_BETA_AXIS);
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