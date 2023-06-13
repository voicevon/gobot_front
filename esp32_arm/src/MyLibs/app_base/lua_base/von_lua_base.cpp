#include "von_lua_base.h"
#include "MyLibs/utility/logger.h"
#include "MyLibs/mqtt/mqtt_subscriber_manager.h"
#include "Mylibs/mqtt/wifi_mqtt_client.h"
#include <SPIFFS.h>

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
			// g_mqttClient.publish("lua/test/output",2, true, s);
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

static const struct luaL_Reg von_hw_functions[] =
{
	{"pinMode", lua_wrapper_pinMode},
    {"digitalWrite", lua_wrapper_digitalWrite},
    {"digitalRead", lua_wrapper_digitalRead},
	{"millis", lua_wrapper_millis},
    {0,           	0}
};

void LuaBase::FeedText(TextMessageLine* text_message_line){

}
void LuaBase::SpinOnce(){
	if (__is_doing_loop){
		// Serial.print("b");
		this->Lua_dostring("loop()");
	}
}
void LuaBase::onGot_MqttMessage(const char* payload, uint16_t payload_len){


	Logger::Debug("LuaBase::onGot_MqttMessage()");
	// char* pp = (char*) payload;
	// *(pp + payload_len) = 0x00;   //??? any risk?
	// write to file
	File file = SPIFFS.open("/test.lua", FILE_WRITE);
	file.println(payload);
	file.close();
	Logger::Info("/test.lua is saved.");


	Logger::Info(payload);
	// do_file in memory
	this->Begin();
	this->Lua_dostring(payload);
	this->Lua_dostring("setup()");
	__is_doing_loop = true;
	// mqttPayloadBuffer.reset();
}

void LuaBase::Link_Mqtt_for_Test(const char* mqtt_topic){
    gs_MqttSubscriberManager::Instance().AddSubscriber(mqtt_topic, this);
}

void LuaBase::Begin(){
	if (Is_running_file){
		// abord currenttly running
	}
	_lua_state = luaL_newstate();
	luaopen_base(_lua_state);
	luaopen_table(_lua_state);
	luaopen_string(_lua_state);
	luaopen_math(_lua_state);
	// register  hardware driver
	lua_register(_lua_state, "print", lua_wrapper_print);
    luaL_openlibs(_lua_state); 
	luaL_newlib(_lua_state, von_hw_functions);
    lua_setglobal(_lua_state, "hw");   


	// register customized driver
	this->_Go_on_register();
}


String LuaBase::Lua_dostring(const char *script) {
	String result;
	if (luaL_dostring(_lua_state, script)) {
		result += "lua::" + String(lua_tostring(_lua_state, -1));
		Logger::Error(result.c_str());
		lua_pop(_lua_state, 1);
		__is_doing_loop = false;
	}
	return result;
}



