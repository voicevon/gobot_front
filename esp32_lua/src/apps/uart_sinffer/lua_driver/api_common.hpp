#pragma once

extern "C"
{

extern void set_callback_mqtt_publish( void (*service_function)(const char*));
extern void set_callback_mqtt_subscribe( void (*service_function)(const char*));
extern void set_callback_mqtt_read_payload( int (*service_function)(char*) );

}