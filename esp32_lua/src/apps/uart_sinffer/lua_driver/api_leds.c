
#include "lua/lua.h"
#include "driver/gpio.h"


#define LED_INDEX_POWER 1



void (*call_cpp_set_led)(int led_index, int state);  

void InitCallback_CppSetLedState(void(*cpp_service_function)(int led_index,int state)){
    call_cpp_set_led = cpp_service_function;
}

int SetLedPower(lua_State* L){
    //约定:  lua_SetLedPower(int state)
    // state { 0:OFF, 1:ON, 2:FLASH}
    int i = lua_gettop(L);
    if(i == 1){
        if(lua_isinteger(L, 1)){
            int state = lua_tointeger(L, 1);
            call_cpp_set_led(LED_INDEX_POWER, state);
        }            
    }

//     return 0;
    gpio_set_level(GPIO_NUM_0, 1);
}
