#pragma once

extern "C"
{

extern void set_callback_mqtt_publish( void (*service_function)(const char*, const char*));
extern void set_callback_mqtt_subscribe( void (*service_function)(const char*, int));

// extern void set_callback_read_first_topic(const char* (*service_function)());
extern void set_callback_mqtt_read_payload( int (*service_function)(int, const char*) );
extern void set_callback_mqtt_release_buffer(void (*service_function) (const int));

}