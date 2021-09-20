#include "CommuBleGattServer.h"
#include "all_devices.h"




//  Concepts       https://www.youtube.com/watch?v=2mePPqiocUE  
//  Notification   https://www.youtube.com/watch?v=oCMOYS71NIU


void CommuBleGattServer::Init(){
  pMyBle =  new MyBleServerCallbacks();
  BLEDevice::init(BLE_DEV_NAME);
//   BLEDevice::init(device_name);



  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(pMyBle);


  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);
  pCharNotification = pService->createCharacteristic(
                                        BLE_RESPONSE_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                        BLECharacteristic::PROPERTY_NOTIFY
  );
  // BLE2902 * pp= new BLE2902();
  // pp->setNotifications(true);
  // pCharResonse->addDescriptor(pp);  
  pCharChattingroom = pService->createCharacteristic(
                                        BLE_COMMAND_UUID,
                                        BLECharacteristic::PROPERTY_WRITE |
                                        BLECharacteristic::PROPERTY_INDICATE
  );

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void CommuBleGattServer::SpinOnce(){
  if ((__State == 2) &&  (pMyBle->is_connected))
      return;

  if (__State == 0){
    BLEDevice::startAdvertising();
    // delay(5000);
    __start_at = XTHAL_GET_CCOUNT();
    __State = 1;
  }else if (__State == 1){
    uint32_t __connecting_time = XTHAL_GET_CCOUNT() - __start_at;
    if(__connecting_time > 2000000){
      // connecting time out!
      __State = 0;
    }
    if (pMyBle->is_connected ){
      __State = 2;
  }
}

}
// void BleServer::AppendGattChar(uint8_t gattCharId, uint8_t bytesCount){
  
// }

bool CommuBleGattServer::HasNewChatting(){
  return this->command_is_new;
}

char* CommuBleGattServer::ReadChatting(){
  command_is_new = false;
}

void CommuBleGattServer::WriteNotification(const char* notification){
  
}

void CommuBleGattServer::OutputMessage(std::string message){
  
}

