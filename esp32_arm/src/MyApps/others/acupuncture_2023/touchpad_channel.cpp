#include "touchpad_channel.h"
#include "MyLibs/basic/logger.h"

void TouchPad_Channel::Init(uint8_t channel_id, EnumState state){
    __channel_id = channel_id;
    __state = state;
    __currently_is_touched = false;
}


bool TouchPad_Channel::Review_Sensor_Value_Whether_Changed(){
    uint8_t newest = __untouched_history_values[0];
    int sum = 0;
    // Logger::Debug("TouchPad_Channel::Review_Sensor_Value_Whether_Changed()");
    for (int i=1; i<5; i++){
        // Logger::Print("debug point   i", i);
        sum += __untouched_history_values[i];
        // Logger::Print("debug point sum", sum);
    }
    int average = sum / 4;
    if (average==0) average = 1;
    if (newest > average * 10) {
        // from untouching to touched.  Don't push new data.
        Logger::Print("got touched", 1);
        __currently_is_touched = true;
        return true; 
    }
    return false;
}


void TouchPad_Channel::Push_to_HistoryValues(uint8_t new_value){
    __capacity_sensor_value = new_value;
    if (__currently_is_touched)
        // the queue is only store untouched sensor value.
        return;

    // __untouched_history_values[5] = __untouched_history_values[4];
    __untouched_history_values[4] = __untouched_history_values[3];
    __untouched_history_values[3] = __untouched_history_values[2];
    __untouched_history_values[2] = __untouched_history_values[1];
    __untouched_history_values[1] = __untouched_history_values[0];
    __untouched_history_values[0] = new_value;

}

void TouchPad_Channel::SetStateToDied(){
    __state = CHANNEL_DIED;
}

String TouchPad_Channel::GetPayloadString(){
    if (__state == NOT_INSTALLED) return String("I");
    if (__state == CHANNEL_DIED) return String("D");
    if (__state == TOUCHED_ON) return String("T");
    if (__state == TOUCHED_OFF) return String("F");

}
