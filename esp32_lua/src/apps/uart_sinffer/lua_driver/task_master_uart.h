#pragma once
#include <lua/lua.h>


extern int LuaUartRecv(lua_State* L);
extern int LuaUartSend(lua_State* L);

extern void Task_MasterUart(void * parameter);
