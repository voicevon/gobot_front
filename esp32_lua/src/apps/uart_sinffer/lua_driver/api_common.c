#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include <lua/lua.h>


// void (*__service_function)(const char*); 
// void def_callback(const char*);

void (*mqtt_publish) (const char*);
void (*mqtt_subscribe) (const char*);
int (*mqtt_read_payload) (char*);

void set_callback_mqtt_publish( void (*service_function)(const char*) ){
	mqtt_publish = service_function;	
}
void set_callback_mqtt_subscribe( void (*service_function)(const char*) ){
	mqtt_subscribe = service_function;	
}
void set_callback_mqtt_read_payload( int (*service_function)(char*) ){
	mqtt_read_payload = service_function;	
}

int LuaMqttSubscribe(lua_State* L){
	uint8_t payload[64];
	uint8_t semcount;
	
	//获得LUA传递过来的数组
	int length = 0;
	int i = lua_gettop(L);
	if(lua_istable(L, i)){
		lua_pushnil(L);
		
		while(lua_next(L, i) != 0){
			payload[length] = lua_tointeger(L, -1);
			lua_remove(L, -1);
			++length;
		}
	}
	mqtt_subscribe(payload);
	
	// do{
	// 	// Wait while UART TX is busy.
	// 	semcount = 0;
	// }while(semcount > 0);
	// uart_write_bytes_with_break(UART_NUM_1, buf, length, 1000);
	return 0;
}

int LuaMqttOnReceived(lua_State* L){

}


int LuaMqttPublish(lua_State* L){
	int i;
	uint8_t payload[64];
	uint8_t semcount;
	
	//获得LUA传递过来的数组
	int length = 0;
	i = lua_gettop(L);
	if(lua_istable(L, i)){
		lua_pushnil(L);
		
		while(lua_next(L, i) != 0){
			payload[length] = lua_tointeger(L, -1);
			lua_remove(L, -1);
			++length;
		}
	}
	
	// do{
	// 	// Wait while UART TX is busy.
	// 	semcount = 0;
	// }while(semcount > 0);
	// uart_write_bytes_with_break(UART_NUM_1, buf, length, 1000);
	return 0;
}