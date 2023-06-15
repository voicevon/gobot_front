#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include <lua/lua.h>


int LuaMqttSubscribe(lua_State* L){

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