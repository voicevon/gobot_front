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

  uint8_t code = 0;
  // pCharacteristic->setValue(&code,1);
  pCharacteristic->setValue("A");
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

int i = 65;

void loop(){
  uint8_t arm_code = 0;
  uint8_t house_code = 0;

  // arm->SpinOnce();
  // arm_code = action.bytes[0];

  // house->SpinOnce();
  // house_code = action.bytes[0];

  // uint8_t code = arm_code + house_code;
  uint8_t code = i;
  pCharacteristic->setValue(&code,3);
  if (action.bytes[0] <= 1){
    // Both head side and Esp side are idle. 
    std::string value = pCharacteristic->getValue();
    uint8_t* data  = pCharacteristic->getData();
    Serial.print("The new characteristic value is: ");
    Serial.println(value.c_str());
    Serial.print(*data);
    // uint8_t* p= (uint8_t* )(&value);
    // Serial.println(*p);
    // Serial.println(*(p+1));
    // Serial.println(*(p+2));
  }
  // Serial.print(i);
  // Serial.print("   ");
  // ble_server->SpinOnce();
  // arm->SpinOnce(&ble_server->body_action);
  // ble_server->UpdateActionCode(i);
  delay(2000);
  i++;
}