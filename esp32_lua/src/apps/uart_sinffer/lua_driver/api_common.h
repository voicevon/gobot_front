#pragma once
#include "lua/lua.h"

extern int LuaMqttPublish(lua_State* L);
extern int LuaMqttSubscribe(lua_State* L);
extern int LuaMqttOnReceived(lua_State* L);
