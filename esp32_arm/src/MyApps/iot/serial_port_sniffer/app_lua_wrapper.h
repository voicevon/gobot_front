#pragma once
#include "Arduino.h"
#pragma comment
#define LUA_USE_C89
// #include "lua/lua.hpp"
#include "MyLibs/app_base/lua_base/lua_wrapper_base.h"

class AppLuaWrapper: public LuaWrapperBase {
  public:
    // AppLuaWrapper();
    // String Lua_dostring(const String *script);
    // String Lua_dofile(const String *filename);
    // void Lua_register(const String name, const lua_CFunction function);

  private:
    // lua_State *_state;
    // String addConstants();
};

