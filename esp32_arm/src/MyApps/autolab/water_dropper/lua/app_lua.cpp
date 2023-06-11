#include "app_lua.h"
#include "../board/board.h"
#include "../board/web_configurator_diction.h"

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
	static int lua_wrapper_readDelaySecond(lua_State *lua) {
		// bool is_pressed = water_dropper_board.Get_ActionButton()->IsFired();
		// lua_pushboolean(lua, is_pressed);
		float seconds = atof(water_dropper_webconfigurator_diction.para_timer.readFile());
		lua_pushnumber(lua, seconds);
		return 1;
	}

	static int lua_wrapper_setMotorSpeed(lua_State* lua){
		int a = luaL_checkinteger(lua, 1);
		water_dropper_board.Get_H_Bridge()->SetPwmSpeed(true, a);
		return 0;
	}
}


static const struct luaL_Reg von_waterdropper_functions[] = {
	{"readButton", lua_wrapper_readButton},
    {"readEncoderAngle", lua_wrapper_readEncoderAngle},
    {"setMotorSpeed", lua_wrapper_setMotorSpeed},
	{"readDelaySecond", lua_wrapper_readDelaySecond},
    {0,           	0}
};


void WaterDropper_LuaWrapper::_Go_on_register(){
	luaL_openlibs(_lua_state); 
	luaL_newlib(_lua_state, von_waterdropper_functions);
    lua_setglobal(_lua_state, "wd");   
}

