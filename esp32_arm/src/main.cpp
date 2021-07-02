#include <Arduino.h>
// #include "ble_server.h"
#include "arm.h"
#include "house.h" 
#include "test.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

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

/*==========================================================================
 * The sketch shows how to move more than one motor. 
 * 
 * If more than one motor is moved by one controller all motors will arrive at 
 * their targets at the same time. E.g., if the motors are part of a 
 * x/y transport system, the transport move on a straight diagonal line to the
 * target coordinates.
 * 
 * The sketch also shows examples how the motor properties are set up
 *
 * A 1/16 microstep driver is assumed. You probably want to adjust speed, 
 * acceleration and distances if you are using a driver with another microstep 
 * resolution.
 ===========================================================================*/

// #include "ESP32Step/TeensyStep.h"
#include "ESP32Step/src/TeensyStep.h"
Stepper motor_1(2, 3);   //STEP pin =  2, DIR pin = 3
Stepper motor_2(9,10);   //STEP pin =  9, DIR pin = 10
Stepper motor_3(14,15);  //STEP pin = 14, DIR pin = 15

StepControl controller;

void setup2()
{
  // setup the motors 
   motor_1
    .setMaxSpeed(50000)       // steps/s
    .setAcceleration(200000); // steps/s^2 
  
  motor_2
    .setMaxSpeed(50000)       // steps/s
    .setAcceleration(200000); // steps/s^2 
  motor_3
    //.setPullInSpeed(300)      // steps/s     currently deactivated...
    .setMaxSpeed(10000)       // steps/s
    .setAcceleration(50000)   // steps/s^2     
    .setStepPinPolarity(LOW); // driver expects active low pulses
}

void loop2() 
{  
  constexpr int spr = 16*200;  // 3200 steps per revolution
  
  // lets shake    
  for(int i = 0; i < 5; i++)
  {
    motor_1.setTargetRel(spr/4); // 1/4 revolution
    controller.move(motor_1);  

    motor_1.setTargetRel(-spr/4);
    controller.move(motor_1);  
  }
  delay(500);
  
  // move motor_1 to absolute position (10 revolutions from zero)
  // move motor_2 half a revolution forward  
  // both motors will arrive their target positions at the same time
  motor_1.setTargetAbs(10*spr);
  motor_2.setTargetRel(spr/2);
  controller.move(motor_1, motor_2);

  // now move motor_2 together with motor_3
  motor_2.setTargetRel(300);
  motor_3.setTargetRel(-800);
  controller.move(motor_2, motor_3);

  // move all motors back to their start positions
  motor_1.setTargetAbs(0);
  motor_2.setTargetAbs(0);
  motor_3.setTargetAbs(0);
  controller.move(motor_1, motor_2, motor_3);
 
  delay(1000);
}



void setup(){
  setup2();
  Serial.begin(115200);
  ble_setup();
  arm->Setup(&action);
  house->Setup(&action);
}

int8_t GetTrueBitIndex(uint8_t any){
  if (any % 2 ==1)
    // is doing a task.
    return -1;

  for(int i=0; i<8; i++){
    if (((any >> i) & 0b00000001) == 1)
      // will start to do a task
      action.bytes[0]++;
      return i;
  }
  // No task at all
  return -2;
}
void loop(){
  loop2();
  // uint8_t arm_code;
  // uint8_t house_code;
  uint8_t house_id;
  arm->SpinOnce();
  // arm_code = action.bytes[0];
  house->SpinOnce();
  // house_code = action.bytes[0];

  if (action.bytes[0] <= 1){
    // Both head side and Esp side are idle. 
    // uint8_t esp_code = arm_code | house_code;
    uint8_t* pData  = pCharacteristic->getData();
    Serial.println("    ");
    for (int i=0; i<13; i++){
      action.bytes[i] = *(pData + i);
      Serial.print(action.bytes[i]);
      Serial.print(" ");
    }
    int8_t true_bit = GetTrueBitIndex(action.bytes[0]);
    Serial.print("gggggggggggggggggg");
    Serial.println(true_bit);
    switch (true_bit){
      //All the below functions will modify action.bytes[0]
      case 2:
        arm->pick_place_park(&action);
        break;
      case 4:
        arm->Home(4);
        break;
      case 5:
        arm->Home(5);
        break;
      case 6:
        house_id = action.House.from_start_house_id;
        house->DrawStone(house_id);
        break;
      case 7:
        house_id = action.House.from_start_house_id;
        house->MoveStoneToTarget(house_id);
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