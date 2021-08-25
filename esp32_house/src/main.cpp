#include <Arduino.h>
// #include "ble_server.h"
// #include "arm.h"
#include "house.h" 
// #include "test.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// BleServer* ble_server = &BleServer::getInstance();
// Arm* arm; 
House* house;
BLECharacteristic *pCharacteristic;
RobotAction action;

void ble_setup(){
  BLEDevice::init("Pefect_Gobot");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
    // BLECharacteristic *pCharacteristic;

  uint8_t code[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
  pCharacteristic->setValue(code,13);
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
  // arm = &Arm::getInstance();
  // arm->Setup(&action);
  // Serial.print("\nArm setup is done.......");
  house = &House::getInstance();
  house->Setup(&action, 9);
  Serial.print("\nHouse setup is done..........");
  ble_setup();
  Serial.print("\nble setup is done......");
  // arm->Home(ARM_ALPHA_AXIS);
  // arm->Home(ARM_BETA_AXIS);
  // Serial.print("\nArm Homing is done......");
  house->Home(HOUSE_ALPHA_AXIS);
  house->Home(HOUSE_BETA_AXIS);
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
  uint8_t house_id;
  // arm->SpinOnce();
  house->SpinOnce();

  if (action.bytes[0] <= 1){
    // Both head side and Esp side are idle. 
    uint8_t* pData  = pCharacteristic->getData();
    // Serial.println("    ");
    for (int i=0; i<14; i++){
      action.bytes[i] = *(pData + i);
      // Serial.print(action.bytes[i]);
      // Serial.print(" ");
    }
    int8_t true_bit = GetTrueBitIndex(action.bytes[0]);
    // Serial.print("true_bit_index=");
    // Serial.println(true_bit);
    switch (true_bit){
      //All the below functions will modify action.bytes[0]
      // case 1:
      //   arm->pick_place_park(&action);
      //   break;
      // case 2:
      //   arm->pick_place_park(&action);
      //   break;
      // case 3:
      //   arm->pick_place_park(&action);
      //   break;
      case 6:
        house_id = action.House.from_start_house_id;
        house->MoveStone_FromRoomToHead(house_id);
        break;
      case 7:
        house_id = action.House.from_start_house_id;
        house->MoveStone_FromHeadToRoom(house_id);
        break;
      // case ARM_ALPHA_AXIS:
      //   arm->Home(ARM_ALPHA_AXIS);
      //   break;
      // case ARM_BETA_AXIS:
      //   arm->Home(ARM_BETA_AXIS);
      //   break;
      case HOUSE_ALPHA_AXIS:
        house->Home(HOUSE_ALPHA_AXIS);
        break;
      case HOUSE_BETA_AXIS:
        house->Home(HOUSE_BETA_AXIS);
        break;
      default:
        break;
    }

    pCharacteristic->setValue(action.bytes,13);

    // if (action.bytes[0] >=2){
    //   Serial.print("hhhhhhhhhhhhhhhhhhhhhhhhhhh    ");
    //   Serial.println(action.bytes[0]);
    // }else{
    //   Serial.print(">>>>>>>>>>>>>>>>>>>>>  ");
    //   Serial.print(action.bytes[0]);
    // }
  }
  delay(500);
}