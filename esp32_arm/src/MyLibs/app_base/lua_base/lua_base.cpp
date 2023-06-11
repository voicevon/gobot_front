#include "lua_base.h"
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
			g_mqttClient.publish("lua/test/output",2, true, s);
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

void LuaWrapperBase::FeedText(TextMessageLine* text_message_line){

}
void LuaWrapperBase::SpinOnce(){
	if (__is_doing_loop){
		this->Lua_dostring("loop()");
	}
}
void LuaWrapperBase::onGot_MqttMessage(const char* payload, uint16_t payload_len){
	Logger::Debug("LuaWrapperBase::onGot_MqttMessage()");
	this->Begin();
	char* pp = (char*) payload;
	*(pp + payload_len) = 0x00;
	// write to file
	File file = SPIFFS.open("/test.lua", FILE_WRITE);
	file.println(payload);
	file.close();

	// do_file in memory
	this->Lua_dostring(payload);
	this->Lua_dostring("setup()");
	Logger::Info("/test.lua is saved.");
	__is_doing_loop = true;
}

void LuaWrapperBase::Link_Mqtt_for_Test(const char* mqtt_topic){
    gs_MqttSubscriberManager::Instance().AddSubscriber(mqtt_topic, this);
}

void LuaWrapperBase::Begin(){
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

// void LuaWrapperBase::LoadString(String* content){
// 	// luaL_loadbuffer(_lua_state,content->c_str(), content->length());
// 	luaL_loadstring(_lua_state, content->c_str());

// }

String LuaWrapperBase::Lua_dostring(const char *script) {
	// Logger::Debug("LuaWrapperBase::Lua_dostring()");
	String result;
	// Logger::Print("Lua_dostirng()", script);
	if (luaL_dostring(_lua_state, script)) {
		result += "# lua error:\n" + String(lua_tostring(_lua_state, -1));
		lua_pop(_lua_state, 1);
	}
	return result;
}

// void LuaWrapperBase::_Lua_register(const String name, const lua_CFunction function) {
// 	lua_register(_lua_state, name.c_str(), function);
// }

