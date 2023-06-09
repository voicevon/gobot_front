#include "app_lua.h"
#include "../board/board.h"

extern "C" {
    static int lua_wrapper_readEncoderAngle_aa(lua_State *lua) {
    // float  angle = serial_port_sniffer_board.Get_Encoder()->getAngle();
    float angle= 123;
    lua_pushnumber(lua, (lua_Number) angle);
    return 1;
  }

}


void SerialPortSniffer_LuaWrapper::_Go_on_register(){
    Logger::Debug("SerialPortSniffer_LuaWrapper::_Go_on_register()");
}


