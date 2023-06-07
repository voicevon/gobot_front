#include "app_lua_wrapper.h"

extern "C" {
  

//   static int lua_wrapper_delay(lua_State *lua) {
//     int a = luaL_checkinteger(lua, 1);
//     delay(a);
//   }

//   static int lua_wrapper_print(lua_State *lua) {
//     String a = String(luaL_checkstring(lua, 1));
//     Serial.println(a);
//   }

void SerialPortSniffer_LuaWrapper::__Go_on_register(){

    // Logger::Debug("SerialPortSniffer_LuaWrapper::__Go_on_register()");
    // _Lua_register("pinMode", (const lua_CFunction) &lua_wrapper_pinMode);
    // _Lua_register("digitalWrite", (const lua_CFunction) &lua_wrapper_digitalWrite);
}


