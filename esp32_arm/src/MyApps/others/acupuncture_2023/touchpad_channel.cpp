#include "touchpad_channel.h"


void TouchPad_Channel::Init(uint8_t cell_id, uint8_t channel_id, EnumState state){
    __cell_id = cell_id;
    __channel_id = channel_id;
    __state = state;
}
