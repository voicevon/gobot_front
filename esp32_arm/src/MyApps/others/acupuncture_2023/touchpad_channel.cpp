#include "touchpad_channel.h"


void TouchPad_Channel::Init(uint8_t cell_id, uint8_t channel_id, EnumState state){
    __cell_id = cell_id;
    __channel_id = channel_id;
    __state = state;
    __currently_is_touched = false;
}


bool TouchPad_Channel::Review_Sensor_Value_Whether_Changed(){
    uint8_t newest = __untouched_history_values[0];
    int sum = 0;
    for (int i=1; i<5; i++){
        sum += __untouched_history_values[i];
    }
    int average = sum / 4;
    if (sum / average * 10) {
        // from untouching to touched.  Don't push new data.
        __currently_is_touched = true;
        return true; 
    }
    return false;
}


void TouchPad_Channel::Push_to_HistoryValues(uint8_t new_value){
    if (__currently_is_touched)
        return;

    __untouched_history_values[5] = __untouched_history_values[4];
    __untouched_history_values[4] = __untouched_history_values[3];
    __untouched_history_values[3] = __untouched_history_values[2];
    __untouched_history_values[2] = __untouched_history_values[1];
    __untouched_history_values[1] = __untouched_history_values[0];
    __untouched_history_values[0] = new_value;

}

void TouchPad_Channel::SetStateTo(EnumState new_state){
    __state = new_state;
}

