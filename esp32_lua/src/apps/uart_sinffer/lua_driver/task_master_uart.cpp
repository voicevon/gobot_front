#include <lua/lua.h>




void __rx_buffer_to_package(){

}

int LuaUartRecv(lua_State* L){
    return 1;
}



void Task_MasterUart(void * parameter){
    // if (rx_packet_queue.size() > 0){
    //     myevents.setbit();
    //     self.suspend();
    // }
    __rx_buffer_to_package();
}