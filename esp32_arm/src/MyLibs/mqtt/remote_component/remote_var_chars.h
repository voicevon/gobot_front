#pragma once
#include "von/utility/mqtt/subscriber/mqtt_subscriber_base.h"

// #include "von/basic/c_string/c_string_base.h"

#include "von/basic/c_string/fs_filename.h"

#define REMOTE_VAR_CHARS_MAX_BYTES_OF_BUFFER 60




class RemoteVar_Chars: public MqttSubscriberBase{
    public:
        void InitFilename( const char* filename);
        bool GotRemoteValue(){return __got_remote_value;};
        char* Get(){return __remote_value;};
        //TODO: ReadLine(int line_number);


    private:
        void _onGot_MqttMessage_whole(const char* payload, size_t payload_len) override;
        bool __got_remote_value = false;
        char __remote_value[REMOTE_VAR_CHARS_MAX_BYTES_OF_BUFFER];
        uint8_t __leds_count;
        bool __write_to_file = false;
        // char __fs_filename[20];
        FsFilename __fs_filename;
};