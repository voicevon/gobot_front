#include "CommuBleGattServer.h"
#include "all_devices.h"




//  Concepts       https://www.youtube.com/watch?v=2mePPqiocUE  
//  Level 1:  Basic  
//                https://www.youtube.com/watch?v=2_vlF_02VXk&t=685s
//  Level 2: Notification
//               https://www.youtube.com/watch?v=oCMOYS71NIU


void CommuBleGattServer::Init(){

  BLEDevice::init(BLE_DEV_NAME);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(&this->myBleServercallbacks);


  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);
  pCharNotification = pService->createCharacteristic(
                                        BLE_STATE_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_NOTIFY
  );
  BLE2902 * p2902a= new BLE2902();
  // pp->setNotifications(true);
  pCharNotification->addDescriptor(p2902a); 

  pCharChattingroom = pService->createCharacteristic(
                                        BLE_COMMU_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_NOTIFY |
                                        BLECharacteristic::PROPERTY_WRITE |
                                        BLECharacteristic::PROPERTY_INDICATE
  );
  BLE2902 * p2902b= new BLE2902();
  pCharChattingroom->addDescriptor(p2902b);
  pCharChattingroom->setCallbacks(&this->myBleCharacteristicCallbacks);
  

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  this->start_timestamp = XTHAL_GET_CCOUNT();

}

void CommuBleGattServer::SpinOnce(){
  if  (this->is_connected){
    return;
  }
    uint32_t connecting_time = XTHAL_GET_CCOUNT() - start_timestamp;
    if(connecting_time > (int32_t)(1900000000)){
      // connecting time out!
      Serial.println("\nBLE Connecting time out, do advertize.");
      BLEDevice::startAdvertising();
      start_timestamp = XTHAL_GET_CCOUNT();
    }
}

bool CommuBleGattServer::HasNewChatting(){
  // TODO: Set command_is_new = true.   When BLE got new GATT message; 
  return this->command_is_new;
}

char* CommuBleGattServer::ReadChatting(){
  command_is_new = false;
  uint8_t* p = pCharChattingroom->getData();
  return (char*)(p);

}

void CommuBleGattServer::WriteNotification(const char* notification){
  this->pCharNotification->setValue(notification);
  
}

void CommuBleGattServer::OutputMessage(std::string message){
  this->pCharNotification->setValue(message);
}

