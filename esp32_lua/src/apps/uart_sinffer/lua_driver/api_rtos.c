#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lua/lua.h"
#include "global_const.h"



static EventGroupHandle_t  my_EventGroup;
static EventBits_t my_BitsToWaitFor = EVENT_BIT_UART_MASTER_RX | EVENT_BIT_UART_SLAVE_RX | EVENT_BIT_UART_WIFI_TCP_RX ;

/// @brief return all the rtos eventGroupBits.
int LuaWaitEvent(lua_State* l){
	
	BaseType_t clearOnExit = pdFALSE;
	BaseType_t watiForAllBits = pdTRUE;
	TickType_t ticksToWait;
	
	EventBits_t result = xEventGroupWaitBits(my_EventGroup, my_BitsToWaitFor, clearOnExit, watiForAllBits, ticksToWait);
	//把flags发送到LUA中
	lua_pushinteger(l, result);
	return 1;
}

void InitRtos(){
	my_EventGroup = xEventGroupCreate();
	if( my_EventGroup == NULL ) {
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
    }else{
        /* The event group was created. */
    }

}
