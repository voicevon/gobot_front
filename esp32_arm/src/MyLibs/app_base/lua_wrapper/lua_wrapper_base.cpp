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
		if (s == NULL)
		return luaL_error(L, "'tostring' must return a string to 'print'");
		if (i>1) Serial.write("\t");
		Serial.write(s);
		lua_pop(L, 1);  /* pop result */
	}
	Serial.println();
	return 0;
	}
} 

void LuaWrapperBase::_InitLua() {
	_state = luaL_newstate();
	luaopen_base(_state);
	luaopen_table(_state);
	luaopen_string(_state);
	luaopen_math(_state);
	lua_register(_state, "print", lua_wrapper_print);
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
void LuaWrapperBase::Begin(const char* filename){
	if (_is_running){
		// abord currenttly running
	}
	this->_InitLua();
	__lua_filename.CopyFrom(filename);
	fs::File xx = SPIFFS.open(__lua_filename.GetChars(), FILE_READ);
	__lua_file = &xx;
	// __lua_file = new(SPIFFS.open(__lua_filename.GetChars(), FILE_READ));

	// memcpy(_filename, filename,20);
	// fs::File file = SPIFFS.open("/lua_script.txt", FILE_READ);
	// if (file) {
	// 	_is_running = true;
	// }else{
	// 	Serial.println("Failed to open file for reading");
	// }
}

void LuaWrapperBase::SpinOnce(){
	if (__lua_file->available()) {
		String line = __lua_file->readStringUntil('\n');
		Logger::Info(line.c_str());
		String result = Lua_dostring(&line);
		Serial.println(result);
	}else{
		__lua_file->close();
		Logger::Info("LuaWrapperBase::SpinOnce()  Run lua file ending report");
	}

}

void LuaWrapperBase::Lua_register(const String name, const lua_CFunction function) {
	lua_register(_state, name.c_str(), function);
}

// String LuaWrapperBase::_addConstants() {
// 	String constants = "INPUT = " + String(INPUT) + "\r\n";
// 	constants += "OUTPUT = " + String(OUTPUT) + "\r\n";
// 	constants += "LOW = " + String(LOW) + "\r\n";
// 	constants += "HIGH = " + String(HIGH) + "\r\n";
// 	return constants;
// }
