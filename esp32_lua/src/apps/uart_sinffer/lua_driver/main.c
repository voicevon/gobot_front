
#include "api_rtos.h"
#include "von_clib/utility/logger/logger.h"


int __main(){
	InitMonitorUart();
	InitRtosTasks();
}