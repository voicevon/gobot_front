#include <Arduino.h>
// #include "ble_server.h"
#include "test.h"


//  Concepts       https://www.youtube.com/watch?v=2mePPqiocUE  
//  Notification   https://www.youtube.com/watch?v=oCMOYS71NIU
//   
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include <esp_log.h>

#define I_AM_ARM
// #define I_AM_HOUSE

#ifdef I_AM_ARM
  #define BLE_DEV_NAME "ConInt-Arm-213401"
  #define SERVICE_UUID "d592c9aa-0594-11ec-9a03-0242ac130003"
  #define ROBOT_STATE_UUID "b7c65186-0610-11ec-9a03-0242ac130003"
  #define ROBOT_ACTION_UUID "c21a1596-0610-11ec-9a03-0242ac130003"

  #include "arm.h"
  Arm* pArm; 
  static char LOG_TAG[]= "BLE-ARM";
#endif

#ifdef I_AM_HOUSE
  #define BLE_DEV_NAME "ConInt-House-213401"
  #define SERVICE_UUID "b416890c-062e-11ec-9a03-0242ac130003"
  #define ROBOT_STATE_UUID "bfa35098-062e-11ec-9a03-0242ac130003"
  #define ROBOT_ACTION_UUID "c52ca230-062e-11ec-9a03-0242ac130003"

  #include "house.h"   
  House* pHouse; 
  static char LOG_TAG[]= "BLE-HOUSE";
#endif



// BLECharacteristic *pCharacteristic;
BLECharacteristic* pCharRobotAction;
BLECharacteristic* pCharRobotState;
RobotAction action;



// class MyNotifyTask: public {
//   void run(void* data){
//     int value = 0;
//     while (1){
//       delay(2000);
//       ESP_LOGD(LOG_TAG, "*** NOTITY: %d ***", value);
//       pCharacteristic->setValue(&value, 1);
//       pCharacteristic->notify();    // <value> will BE PUSHED from BLEServer to BLEClient.
//       value++;
//     }  //end of while
//   }  //end of function
// }; //end of class


// MyNotifyTask* pMyNotifyTask;


bool is_connected = false;


void NotifyClient(uint8_t state){
  if (is_connected){
    // pCharacteristic->setValue(&state, 1);
    // pCharacteristic->notify();
    pCharRobotState->setValue(&state, 1);
    pCharRobotState->notify();
  }
  else
    ESP_LOGW(LOG_TAG, "Can not notify,  No BLEClient is connected!");
}
class MyBleServerCallbacks: public BLEServerCallbacks{
  void onConnected(BLEServer* pServer){
    // pMyNotifyTask->start();
    is_connected = true;
    ESP_LOGD(LOG_TAG, "Client is connected!");
  }
  void onDisconnected(BLEServer* pServer){
    // pMyNotifyTask->stop();
    is_connected = false;
    ESP_LOGD(LOG_TAG, "Client is DIS-connected!");
  }
};


void ble_setup(){
  // pMyNotifyTask = new MyNotifyTask();

  BLEDevice::init(BLE_DEV_NAME);
  // BLE::initServer(BLE_DEV_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  // BLEServer *pServer = new BLEServer();
  pServer->setCallbacks(new MyBleServerCallbacks);


  BLEService *pService = pServer->createService(SERVICE_UUID);
  // BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));
  // pCharacteristic = pService->createCharacteristic(
  //                                        CHARACTERISTIC_UUID,
  //                                        BLECharacteristic::PROPERTY_READ |
  //                                        BLECharacteristic::PROPERTY_WRITE |
  //                                        BLECharacteristic::PROPERTY_NOTIFY
  //                                      );
  // BLECharacteristic *pCharacteristic;
  // pCharacteristic->addDescriptor(new BLE2902());
  pCharRobotState = pService->createCharacteristic(
                                        ROBOT_STATE_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharRobotState->addDescriptor(new BLE2902());  
  pCharRobotAction = pService->createCharacteristic(
                                        ROBOT_ACTION_UUID,
                                        BLECharacteristic::PROPERTY_WRITE |
                                        BLECharacteristic::PROPERTY_INDICATE
  );

  // uint8_t code[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
  // pCharacteristic->setValue(code,13);
  // pCharacteristic->setValue("A");


  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void setup(){
  Serial.begin(115200);
  Serial.print("\n Gobot system is starting....");
  ble_setup();
  Serial.print("\nble setup is done......");

  #ifdef I_AM_ARM
    pArm = &Arm::getInstance();
    pArm->Setup(&action);
    Serial.print("\nArm setup is done.......");
    pArm->Home(ARM_ALPHA_AXIS);
    pArm->Home(ARM_BETA_AXIS);
    Serial.print("\nArm Homing is done......");
  #endif
  #ifdef I_AM_HOUSE
    pHouse = &House::getInstance();
    pHouse->Setup(&action, 9);
    Serial.print("\nHouse setup is done..........");
    pHouse->Home(HOUSE_ALPHA_AXIS);
    pHouse->Home(HOUSE_BETA_AXIS);
    Serial.print("\nHouse Homing is done......");
  #endif

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

  if (action.bytes[0] <= 1){
    // Both head side and Esp side are idle. 
    // uint8_t* pData  = pCharacteristic->getData();
    uint8_t* pData  = pCharRobotAction->getData();
    // Serial.println("    ");
    for (int i=0; i<14; i++){
      action.bytes[i] = *(pData + i);
      // Serial.print(action.bytes[i]);
      // Serial.print(" ");
    }
    return;
  }
  
  int8_t true_bit  = GetTrueBitIndex(action.bytes[0]);

  #ifdef I_AM_ARM
    pArm->SpinOnce();

    // Serial.print("true_bit_index=");
    // Serial.println(true_bit);
    switch (true_bit){
      //All the below functions will modify action.bytes[0]
      case 1:
        NotifyClient(1);
        pArm->pick_place_park(&action);
        NotifyClient(0);
        break;

      case 2:
        NotifyClient(2);
        pArm->pick_place_park(&action);
        NotifyClient(0);
        break;

      case 3:
        NotifyClient(3);
        pArm->pick_place_park(&action);
        NotifyClient(0);
        break;

      case ARM_ALPHA_AXIS:
        NotifyClient(ARM_ALPHA_AXIS);
        pArm->Home(ARM_ALPHA_AXIS);
        NotifyClient(0);
        break;
      case ARM_BETA_AXIS:
        NotifyClient(ARM_BETA_AXIS);
        pArm->Home(ARM_BETA_AXIS);
        NotifyClient(0);
        break;

      default:
        break;
    }
  #endif

  #ifdef I_AM_HOUSE
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
  #endif
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