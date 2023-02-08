#include "touchpad_channel.h"
#include "MyLibs/basic/logger.h"


#define CHANNEL_DIED_CODE 254
#define CHANNEL_NOT_INSTALLED_CODE 255   // useless ?

void TouchPad_Channel::Init(uint8_t channel_id, EnumState state){
    __channel_id = channel_id;
    __state = state;
    
}


void TouchPad_Channel::Review_Sensor_Value(uint8_t new_value){
    // Logger::Debug("TouchPad_Channel::Review_Sensor_Value()   Part 1");
    __newest_sensor_value = new_value;
    if(__state == EnumState::NOT_INSTALLED)  return;
    if (__newest_sensor_value == CHANNEL_DIED_CODE ){
        __state = EnumState::CHANNEL_DIED;
        return;
    }

    int sum = 0;
    // Logger::Debug("TouchPad_Channel::Review_Sensor_Value()  Part 2");
    for (int i=0; i<SENSOR_HISTORY_QUEUE_SIZE; i++){
        // Logger::Print("debug point   i", i);
        sum += __untouched_history_values[i];
        // Logger::Print("debug point sum", sum);
    }
    int average = sum / SENSOR_HISTORY_QUEUE_SIZE;
    if (average==0) average = 1;
    if ((__state == EnumState::TOUCHED_OFF) && (__newest_sensor_value > average * 10)) {
        // from untouching to touched.  Don't push new data to ihstory queue.
        Logger::Debug("ouchPad_Channel::Review_Sensor_Value_Whether_Changed()  Got touched");
        Logger::Print("average", average);
        Logger::Print("newest", __newest_sensor_value);
        __state = EnumState::TOUCHED_ON;
    }
    if ((__state == EnumState::TOUCHED_ON) && (__newest_sensor_value < average * 5)){
        Logger::Debug("ouchPad_Channel::Review_Sensor_Value_Whether_Changed()  Got Untouched");
        Logger::Print("average", average);
        Logger::Print("newest", __newest_sensor_value);
        __state = EnumState::TOUCHED_OFF;
    }
    if (__state == EnumState::TOUCHED_OFF){
        
    // Logger::Print("TouchPad_Channel::Review_Sensor_Value()  point",  33);
        __Push_to_HistoryValueWindow(new_value);
    }
}


void TouchPad_Channel::__Push_to_HistoryValueWindow(uint8_t new_value){
    // Logger::Debug("TouchPad_Channel::__Push_to_HistoryValueWindow()");
    for(int i=0; i< SENSOR_HISTORY_QUEUE_SIZE -1; i++){
        __untouched_history_values[i] = __untouched_history_values[i+1];
    }
    __untouched_history_values[SENSOR_HISTORY_QUEUE_SIZE -1] = new_value;
    // Logger::Print("TouchPad_Channel::__Push_to_HistoryValueWindow()   point",99);
}

String TouchPad_Channel::GetStateString(){
    if (__state == NOT_INSTALLED) return String("I");
    if (__state == CHANNEL_DIED) return String("D");
    if (__state == TOUCHED_ON) return String("T");
    // if (__state == TOUCHED_OFF) return String("F");
    return "F";
}