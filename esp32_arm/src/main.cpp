#include <Arduino.h>
// #include "ble_server.h"
#include "arm.h"
#include "house.h" 
#include "test.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// BleServer* ble_server = &BleServer::getInstance();
Arm* arm = &Arm::getInstance();
House* house = &House::getInstance();
BLECharacteristic *pCharacteristic;
RobotAction action;

void ble_setup(){
  Serial.println("Starting BLE work!");
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
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

// void ble_SetActionCode(unsigned char new_code){
//   RobotAction action;
//   size_t len = sizeof(RobotAction);
//   action.Arm.action_code = new_code;
//   pCharacteristic->setValue(action.bytes, len);
// }


void setup(){
  Serial.begin(115200);
  ble_setup();
  arm->Setup(&action);
  house->Setup(&action);
}

void loop(){
  uint8_t arm_code;
  uint8_t house_code;

  arm->SpinOnce();
  arm_code = action.bytes[0];
  // house->SpinOnce();
  // house_code = action.bytes[0];

  if (action.bytes[0] <= 1){
    // Both head side and Esp side are idle. 
    uint8_t esp_code = arm_code | house_code;
    uint8_t* pData  = pCharacteristic->getData();
    Serial.println("    ");
    for (int i=0; i<13; i++){
      action.bytes[i] = *(pData + i);
      Serial.print(action.bytes[i]);
      Serial.print(" ");
    }
    action.bytes[0] = esp_code;
    pCharacteristic->setValue(action.bytes,13);

    if (action.bytes[0] >=2){
      Serial.print("hhhhhhhhhhhhhhhhhhhhhhhhhhh    ");
      Serial.println(action.bytes[0]);
    }else{
      Serial.print(">>>>>>>>>>>>>>>>>>>>>  ");
      Serial.print(action.bytes[0]);
    }
  }
  delay(500);
}