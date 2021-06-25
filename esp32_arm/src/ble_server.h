#pragma once
#include "actions.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"



class BleServer {
  
  protected:

  public:
     static BleServer& getInstance()
        {
            static BleServer instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
    BodyAction body_action;
    void Init(void);
    void SpinOnce(void);
    void UpdateActionCode(unsigned char new_code);

  private:
    BleServer(){};
    BleServer(BleServer const& copy);            // Not Implemented
    BleServer& operator=(BleServer const& copy); // Not Implemented

    BLECharacteristic *__pCharacteristic;

};