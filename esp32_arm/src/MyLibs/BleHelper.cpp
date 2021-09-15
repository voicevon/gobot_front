#include "BleHelper.h"
#include "all_devices.h"




//  Concepts       https://www.youtube.com/watch?v=2mePPqiocUE  
//  Notification   https://www.youtube.com/watch?v=oCMOYS71NIU


void BleHelper::InitBle(){
  pMyBle =  new MyBleServerCallbacks();
  BLEDevice::init(BLE_DEV_NAME);
//   BLEDevice::init(device_name);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(pMyBle);


  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharRobotState = pService->createCharacteristic(
                                        ROBOT_STATE_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_NOTIFY
  );
  // BLE2902 * pp= new BLE2902();
  // pp->setNotifications(true);
  // pCharRobotState->addDescriptor(pp);  
  pCharRobotAction = pService->createCharacteristic(
                                        ROBOT_ACTION_UUID,
                                        BLECharacteristic::PROPERTY_WRITE |
                                        BLECharacteristic::PROPERTY_INDICATE
  );

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}
