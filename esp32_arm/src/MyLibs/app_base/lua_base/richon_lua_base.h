// #pragma once

// #define LUA_USE_C89
// #include "lua/lua.hpp"

// EventGroupHandle_t my_events;


// int LuaWaitEvent(lua_State* l){
// 	INT8U err;
// 	// OS_FLAGS flags;
	
// 	// flags = OSFlagPend(pFlagGrp, (OS_FLAGS)ALL_FLAGS, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 4, &err);	//50毫秒一次超时
// 	int flags = {12345};

// 	//把flags发送到LUA中
// 	lua_pushinteger(l, flags);
// 	return 1;
// }

// static int LuaUartSend(lua_State* l){
// 	int i, tmp;
// 	INT8U buf[64];
// 	INT16U semcount;
	
// 	//获得LUA传递过来的数组
// 	tmp = 0;
// 	i = lua_gettop(l);
// 	if(lua_istable(l, i)){
// 		lua_pushnil(l);
		
// 		while(lua_next(l, i) != 0){
// 			buf[tmp] = lua_tointeger(l, -1);
// 			lua_remove(l, -1);
// 			++tmp;
// 		}
// 	}
	
// 	do{
// 		// Wait while UART TX is busy.
// 		semcount = OSSemAccept(pUart4SendCompleted);
// 	}while(semcount > 0);
		
// 	SendToUart4(buf, tmp);
	
// 	return 0;
// }


// void init(){
// 	my_events = xEventGroupCreate();
// }