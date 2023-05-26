#include "touchpad_node.h"
#include "MyLibs/basic/logger.h"

void TouchPad_Node::Init( uint8_t i2c_slave_address, bool is_installed){
    _Init(is_installed, i2c_slave_address, TOUCH_PAD_SENSORS_COUNT_IN_NODE);   //TODO: TOUCH_PAD_SENSORS_COUNT_IN_NODE is var

    for(int i=0; i<TOUCH_PAD_SENSORS_COUNT_IN_NODE; i++){
        __all_sensors[i].Init(i, TouchSensor::EnumState::TOUCHED_OFF);
    }
    _rx_buffer = __rx_buffer;
    
}

void TouchPad_Node::Process_RxBuffer(){
    for (int i=0; i<TOUCH_PAD_SENSORS_COUNT_IN_NODE; i++){
        // review sensor's value
        __all_sensors[i].Review_Sensor_Value(*(_rx_buffer+i));
    }
}

char TouchPad_Node::GetState(){
//    C = Connected -> Online
//    D = Died --> Offline
//    I = Installed, Unknown state ?? 
//    U = Uninstalled,
    // if(State == EnumState::ONLINE_CONNECTED) return String("C");
    // if(State == EnumState::OFFLINE_DIED) return String("D");
    if(State == EnumState::ONLINE_CONNECTED) return 'C';
    if(State == EnumState::OFFLINE_DIED) return 'D';
    Logger::Error("TouchPad_Node::GetState()");
    Logger::Print("My I2c_addr", this->_Get_i2c_Addr());
    Logger::Print("Semms like", "I am not installed. Why call me");
    Logger::Halt("We should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil.");
    // return String(" ");
    return 'X';

}

int TouchPad_Node::GetUpdatedSensor(){
    for (int i=0; i<14;i++){
        if (__previous_sensros_state[i] != __all_sensors[i].GetState()){
            __previous_sensros_state[i] = __all_sensors[i].GetState();
            return i;
        }
    }
    return -1;
}


String TouchPad_Node::GetSensorsState(){
    String all_sensors_state = "";
    TouchSensor* sensor;
    if (State != EnumState::ONLINE_CONNECTED){
        return "FFFFFFFFFFFFFF";
    }
    for (int i=0; i<TOUCH_PAD_SENSORS_COUNT_IN_NODE; i++){
        sensor = &__all_sensors[i];
        all_sensors_state.concat(sensor->GetState());
    }
    return all_sensors_state;
}


