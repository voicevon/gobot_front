#pragma once

#include "Arduino.h"
#define LUA_USE_C89
#include "lua/lua.hpp"
#include "von/utility/logger.h"
#include "von/utility/mqtt/subscriber/mqtt_subscriber_base.h"

#include "MyLibs/app_base/text_message_queue/text_message_line.h"

//TODO: no longer a subclass of MqttSubscriberBase
class LuaBase: public MqttSubscriberBase{
	public:
		void FeedText(TextMessageLine* text_message_line);
		String Lua_dostring(const char *script);
		void Begin();
		void Link_Mqtt_for_Test(const char* mqtt_topic);
		bool Is_running_file = false;
		void SpinOnce();
		
	protected:
		virtual void _Go_on_register();
		lua_State *_lua_state;

	private:
		void _onGot_MqttMessage_whole(const char* payload, size_t payload_len) override;

		bool __is_doing_loop = false;

};

