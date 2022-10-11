#include "teeth_wh_board.h"


#define PIN_HOMER_SENSOR_HALL_0 22
#define PIN_HOMER_SENSOR_HALL_1 22
#define PIN_HOMER_SENSOR_HALL_2 22
#define PIN_VACUUME_PUMP 22

void TeethWarehouse_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am Teeth Warehouse.");
    }

    __all_position_triggers[0].Init(PIN_HOMER_SENSOR_HALL_0, LOW);
    __all_position_triggers[1].Init(PIN_HOMER_SENSOR_HALL_1, LOW);
    __all_position_triggers[2].Init(PIN_HOMER_SENSOR_HALL_2, LOW);
    HomeTrigger_Array::Instance().Init(__all_position_triggers, 4);
    
    pinMode(PIN_VACUUME_PUMP, OUTPUT);
}

// void TeethWarehouse_Board::LinkEncoderSensor(Encoder* encoder){
//     this->__motor_angle_sensor.LinkRawSensor(encoder);
// }

void TeethWarehouse_Board::Test_PositionTriggers(int loops){
    uint32_t flags = 0;
    uint32_t last_flags = 999;
    int count =0;
    while (count < loops){
        if (flags != last_flags){
            Serial.print("Trigger is channged: \t\t");
            Serial.println(flags,BIN);
            last_flags = flags;
            count++;
        }
    }
}

void TeethWarehouse_Board::EnableVacuume(bool enable_it){
    digitalWrite(PIN_VACUUME_PUMP, enable_it);
}

