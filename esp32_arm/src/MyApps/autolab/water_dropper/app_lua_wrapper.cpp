#include "app_lua_wrapper.h"
#include "board/board.h"

extern "C" {
  static int lua_wrapper_readEncoderAngle(lua_State *lua) {
    float  angle = water_dropper_board.Get_Encoder()->getAngle();
    lua_pushnumber(lua, (lua_Number) angle);
    return 1;
  }

  static int lua_wrapper_readButton(lua_State *lua) {
    bool is_pressed = water_dropper_board.Get_ActionButton()->IsFired();
    lua_pushboolean(lua, is_pressed);
    return 1;
  }
}

void WaterDropper_LuaWrapper::__Go_on_register(){
	_Lua_register("readButton",lua_wrapper_readButton);
	_Lua_register("readEncoderAngle",lua_wrapper_readEncoderAngle);
}

