/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/


#include "ble_server.h"
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/


void BleServer::Init() {
  Serial.println("Starting BLE work!");
  BLEDevice::init("gobot_esp_arm");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  __pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
    // BLECharacteristic *__pCharacteristic;


  __pCharacteristic->setValue("Hello World says Neil");
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

void BleServer::UpdateActionCode(unsigned char new_code){
  ArmAction aaa;
  size_t len = sizeof(ArmAction);
  aaa.Attr.action_code = new_code;

  __pCharacteristic->setValue(aaa.bytes, len);
}


void BleServer::SpinOnce() {
  int i=0;
  // put your main code here, to run repeatedly:
  std::string value = __pCharacteristic->getValue();
  Serial.print("The new characteristic value is: ");
  Serial.println(value.c_str());
  i++;
  if (i % 2 ==0)
    __pCharacteristic->setValue("ABC");
  else
    __pCharacteristic->setValue("123");
    

}
