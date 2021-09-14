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
// #include <BLE.h>

#include <esp_log.h>
#include "MyBleServerCallbacks.h"

// #define I_AM_GOBOT_ARM
#define I_AM_GOBOT_HOUSE
#define I_AM_CABLE_AXIS


 #include "SingleAxis.h"


#ifdef I_AM_CABLE_AXIS
  #include "SingleAxis.h"
  #define CABLE_AXIS_XPYP
  // #define CABLE_AXIS_XNYP
  // #define CABLE_AXIS_XNYN
  // #define CABLE_AXIS_XPYN
#endif


#ifdef I_AM_GOBOT_ARM
  #define BLE_DEV_NAME "ConInt-Arm-213401"
  #define SERVICE_UUID "d592c9aa-0594-11ec-9a03-0242ac130003"
  #define ROBOT_STATE_UUID "b7c65186-0610-11ec-9a03-0242ac130003"
  #define ROBOT_ACTION_UUID "c21a1596-0610-11ec-9a03-0242ac130003"

  #include "arm.h"
  Arm* pArm; 
  static char LOG_TAG[]= "BLE-ARM";

#endif

#ifdef I_AM_GOBOT_HOUSE
  #define BLE_DEV_NAME "ConInt-House-213401"
  #define SERVICE_UUID "b416890c-062e-11ec-9a03-0242ac130003"
  #define ROBOT_STATE_UUID "bfa35098-062e-11ec-9a03-0242ac130003"
  #define ROBOT_ACTION_UUID "c52ca230-062e-11ec-9a03-0242ac130003"

  #include "house.h"   
  House* pHouse; 
  static char LOG_TAG[]= "BLE-HOUSE";
#endif

#ifdef I_AM_CABLE_AXIS_XPYP
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "1fd03a14-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "4b7fbe96-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "50711238-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XNYP
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "6909288a-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "6e0d5d7e-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "72838810-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XNYN
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "77d37e60-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "8fdb27b0-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "8175103c-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XPYN
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "85f5ab94-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "96a1cd38-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "9c43b29c-1451-11ec-82a8-0242ac130003"
#endif

BLECharacteristic* pCharRobotAction;
BLECharacteristic* pCharRobotState;
RobotAction action;

MyBleServerCallbacks* pMyBle;

void NotifyClient(uint8_t state){
  if (pMyBle->is_connected){
    pCharRobotState->setValue(&state, 1);
    pCharRobotState->notify();
  }
  else
    ESP_LOGW(LOG_TAG, "Can not notify,  No BLEClient is connected!");
}




void ble_setup(){
  pMyBle =  new MyBleServerCallbacks();
  BLEDevice::init(BLE_DEV_NAME);
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

void setup_display(){
    Serial.println("I am House, Testing LED");
    pinMode(LED_A, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(LED_C, OUTPUT);
    pinMode(LED_F, OUTPUT);
    pinMode(LED_H, OUTPUT);

    for(int i=0 ; i<3; i++){
      digitalWrite(LED_A, HIGH);
      digitalWrite(LED_B, HIGH);
      digitalWrite(LED_C, HIGH);
      digitalWrite(LED_F, HIGH);
      digitalWrite(LED_H, HIGH);
      Serial.print (" X ");
      delay(1000);
      digitalWrite(LED_A, LOW);
      digitalWrite(LED_B, LOW);
      digitalWrite(LED_C, LOW);
      digitalWrite(LED_F, LOW);
      digitalWrite(LED_H, HIGH);
      Serial.print (" o ");
      delay(1000);
    }
}

void test(){
  SingleAxis  axis_A(6,8,8) ;
  // axis_A = SingleAxis(2,3,5);
  SingleAxis* xx= &axis_A;
  // xx->Setup([]{xx->ISR_Encoder();});
  // axis_A.Setup([]{axis_A.ISR_Encoder();}); 
  // std::function<void(void) aa;
  void (*aa)();
  // aa = (void(*)()) (axis_A->ISR_Encoder());
  //axis_A->setup2(aa);  
  // attachInterrupt(8, axis_A.ISR_Encoder, RISING);

}
void setup(){
  Serial.begin(115200);
  setup_display();


  
  Serial.print("\nGobot system is starting....");
  ble_setup();
  Serial.print("\nBLE setup is done......");

  #ifdef I_AM_GOBOT_ARM
    pArm = &Arm::getInstance();
    pArm->Setup(&action);
    Serial.print("\nArm setup is done.......");
    pArm->Home(ARM_ALPHA_AXIS);
    pArm->Home(ARM_BETA_AXIS);
    Serial.print("\nArm Homing is done......");
  #endif
  #ifdef I_AM_GOBOT_HOUSE
    pHouse = &House::getInstance();
    // pHouse->Test_home_sensor();


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

  #ifdef I_AM_GOBOT_ARM
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

  #ifdef I_AM_GOBOT_HOUSE
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