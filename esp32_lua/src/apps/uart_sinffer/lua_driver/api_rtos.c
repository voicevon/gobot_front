#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"


#include "lua/lua.h"
#include "global_const.h"
#include "von_clib/utility/logger/logger.h"

#include "task_master_uart.h"
#include "task_slave_uart.h"
#include "task_lua.h"

TaskHandle_t task_MasterUart;
TaskHandle_t task_SlaveUart;
TaskHandle_t task_WifiMqtt;
TaskHandle_t task_LuaVM;

EventGroupHandle_t  my_EventGroup;
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

int LuaResumeTask(lua_State* L){
	int i;
	int pri;
    i = lua_gettop(L);
    if(i == 1)
    {
        if(lua_isinteger(L, 1))
        {
            int task_id = lua_tointeger(L, 1);
			if (task_id == 0){
				xEventGroupClearBits(my_EventGroup, EVENT_BIT_UART_MASTER_RX);
				// vTaskResume(task_MasterUart);
			}else if(task_id == 1){
				xEventGroupClearBits(my_EventGroup, EVENT_BIT_UART_SLAVE_RX);
				// vTaskResume(task_SlaveUart);
			}else if(task_id == 2){
				// vTaskResume(task_WifiTcp);
				xEventGroupClearBits(my_EventGroup, EVENT_BIT_UART_WIFI_TCP_RX);
			}else{
				logWarn("LuaResumeTask()  Lua call me with unknown parameter....\n");
			}
        }            
    }
	
    return 0;	

}

void InitRtosTasks(){
	my_EventGroup = xEventGroupCreate();
	if( my_EventGroup == NULL ) {
        /* The event group was not created because there was insufficient
        FreeRTOS heap available. */
    }else{
        /* The event group was created. */
    }
	logInfo("InitRTOS() is done...\n");

	//create tasks

	xTaskCreate(Task_MasterUart, /* Task function. */
				"Master UART", /* name of task. */
				10000, /* Stack size of task */
				NULL, /* parameter of the task */
				1, /* priority of the task */
				task_MasterUart); 

	xTaskCreate(Task_SlaveUart, "Slave UART", 10000, NULL, 1, task_SlaveUart); 
	// xTaskCreate(Task_WifiMqtt, "Web socket", 10000, NULL, 1, task_WifiMqtt); 
	xTaskCreate(Task_LuaVM, "Lua-VM", 10000, NULL,  1,task_LuaVM);   // ??? stack size?
	logInfo("All RTOS-tasks are created.\n");
}
