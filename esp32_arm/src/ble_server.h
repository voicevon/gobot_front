#pragma once


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

  struct ArmAction{
    unsigned char action_code;
    int16_t pickup_x;
    int16_t pickup_y;

  };
  
class BleServer {


  
  protected:

  public:

    ArmAction arm_action;
    void Init(void);
    void SpinOnce(void);
    void UpdateActionCode(unsigned char new_code);

};