#pragma once

#define LUA_USE_C89
#include "lua/lua.hpp"

EventGroupHandle_t my_events;


int LuaWaitEvent(lua_State* l){
	INT8U err;
	// OS_FLAGS flags;
	
	// flags = OSFlagPend(pFlagGrp, (OS_FLAGS)ALL_FLAGS, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 4, &err);	//50毫秒一次超时
	int flags = {12345};

	//把flags发送到LUA中
	lua_pushinteger(l, flags);
	return 1;
}

void init(){
	my_events = xEventGroupCreate();
}