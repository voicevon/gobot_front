#include "cnc_drivers.h"

void CncDriver_CoreYZ_2205::Init(){
    Serial.println("[Error] CncDriver_CoreYZ_2205::Init().  You should call Init(Adafruit_MCP23X17* )");
}

void CncDriver_CoreYZ_2205::Init(Adafruit_MCP23X17* mcp_23018){
    this->__mcp23018 = mcp_23018;
    this->stepper_alpha.Init_mcp23018(mcp_23018, MC23018_PIN_ALPHA_DIR_2205);
    this->stepper_beta.Init_mcp23018(mcp_23018, MC23018_PIN_BETA_DIR_2205);
    this->homer_y.Init_mcp23018(mcp_23018, MC23018_PIN_HOME_Y_2205);
    this->homer_z.Init_mcp23018(mcp_23018, MC23018_PIN_HOME_Z_2205);
}

Stepper* CncDriver_CoreYZ_2205::GetStepper(char axis_name){
    if (axis_name=='A'){
        return &this->stepper_alpha;
    }else if (axis_name=='B'){
        return &this->stepper_beta;
    }else{
        Serial.print("['Error']  Board_GobotHouse::GetStepper()   axis_name= ");
        Serial.println(axis_name);
    }
}

SingleAxisHomer* CncDriver_CoreYZ_2205::GetHomer(char axis_name){
    if (axis_name=='Z'){
        return &this->homer_z;
    }else if (axis_name=='Y'){
        return &this->homer_y;
    }else{
        Serial.print("['Error']  Board_GobotHouse::GetHomer()   axis_name= ");
        Serial.println(axis_name);
    }
}

RobotEef_GarmentAsar* CncDriver_CoreYZ_2205::GetEef(){
    return &this->eef;
}

void CncDriver_CoreYZ_2205::EnableMotor(char axis_name, bool enable_it){
    if (axis_name == 'A'){
        this->__mcp23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE_2205, !enable_it);   // LOW is enable
    } else if (axis_name == 'B'){
        this->__mcp23018->digitalWrite(MC23018_PIN_BETA_ENABLE_2205, !enable_it);   // LOW is enable
    } else {
        Serial.println("[Warn] CncDriver_CoreYZ_2205::EnableMotor()  ");
    }
}

