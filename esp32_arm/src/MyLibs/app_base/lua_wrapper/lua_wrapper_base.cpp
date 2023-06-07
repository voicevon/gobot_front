#include "lua_wrapper_base.h"
#include "MyLibs/utility/logger.h"

extern "C" {
	static int lua_wrapper_print (lua_State *L) {
		int n = lua_gettop(L);  /* number of arguments */
		int i;
		lua_getglobal(L, "tostring");
		for (i=1; i<=n; i++) {
			const char *s;
			size_t l;
			lua_pushvalue(L, -1);  /* function to be called */
			lua_pushvalue(L, i);   /* value to print */
			lua_call(L, 1, 1);
			s = lua_tolstring(L, -1, &l);  /* get result */
			if (s == NULL){
				return luaL_error(L, "'tostring' must return a string to 'print'");
			}
			if (i>1) {
				Serial.write("\t");
			}
			Serial.write(s);
			lua_pop(L, 1);  /* pop result */
		}
		Serial.println();
		return 0;
	}
		static int lua_wrapper_pinMode(lua_State *lua) {
		int a = luaL_checkinteger(lua, 1);
		int b = luaL_checkinteger(lua, 2);
		pinMode(a, b);
		return 0;
	}

	static int lua_wrapper_digitalWrite(lua_State *lua) {
		int a = luaL_checkinteger(lua, 1);
		int b = luaL_checkinteger(lua, 2);
		digitalWrite(a, b);
		return 0;
	}
	static int lua_wrapper_digitalRead(lua_State *lua) {
		int a = luaL_checkinteger(lua, 1);
		lua_pushnumber(lua, (lua_Number)digitalRead(a));
		return 1;
	}

	static int lua_wrapper_millis(lua_State *lua) {
		lua_pushnumber(lua, (lua_Number) millis());
		return 1;
	}

} 

void LuaWrapperBase::Begin(){
	if (_is_running){
		// abord currenttly running
	}

	_state = luaL_newstate();
	luaopen_base(_state);
	luaopen_table(_state);
	luaopen_string(_state);
	luaopen_math(_state);
	// register  common_driver
	lua_register(_state, "print", lua_wrapper_print);
	_Lua_register("pinMode", lua_wrapper_pinMode);
    _Lua_register("digitalWrite", lua_wrapper_digitalWrite);
    _Lua_register("digitalRead", lua_wrapper_digitalRead);
	_Lua_register("millis", lua_wrapper_millis);

	// register customized driver
	this->__Go_on_register();
	_is_running = true;
}

void LuaWrapperBase::_InitLua() {

}

String LuaWrapperBase::Lua_dostring(const String *script) {

	// String scriptWithConstants = _addConstants() + *script;
	String scriptWithConstants = *script;
	String result;
	if (luaL_dostring(_state, scriptWithConstants.c_str())) {
		result += "# lua error:\n" + String(lua_tostring(_state, -1));
		lua_pop(_state, 1);
	}
	return result;
}

String LuaWrapperBase::Lua_dostring(const char *script) {

	// String scriptWithConstants = _addConstants() + *script;
	// String scriptWithConstants = *script;
	String result;
	if (luaL_dostring(_state, script)) {
		result += "# lua error:\n" + String(lua_tostring(_state, -1));
		lua_pop(_state, 1);
	}
	return result;
}

void LuaWrapperBase::_Lua_register(const String name, const lua_CFunction function) {
	lua_register(_state, name.c_str(), function);
}


