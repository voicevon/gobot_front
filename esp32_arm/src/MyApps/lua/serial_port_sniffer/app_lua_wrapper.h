#pragma once
#include "Arduino.h"
#pragma comment
#define LUA_USE_C89
#include "lua/lua.hpp"

class AppLuaWrapper {
  public:
    AppLuaWrapper();
    String Lua_dostring(const String *script);
    void Lua_register(const String name, const lua_CFunction function);

  private:
    lua_State *_state;
    String addConstants();
};

