

// https://github.com/theElementZero/ESP32-UAexRT-interrupt-handling/blob/master/uart_interrupt.c
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"
#include "esp32/rom/uart.h"   //https://stackoverflow.com/questions/62453460/esp32-uart-interrupt


static const char *TAG = "master_uart";
// char END_OF_LINE = 0x0a;
// char END_OF_LINE[] = "\n";


#define EX_UART_NUM UART_NUM_0
#define BUF_SIZE (1024)


void logWarn(const char* title){

}

void logInfo(const char* title){
	for(int i=0; i<100; i++){
		if (title[i] == 0x00){
			// uart_write_bytes(UART_NUM_0, END_OF_LINE, 1);     //??? 
			// uart_write_bytes(UART_NUM_0, &END_OF_LINE, 1);    //???
			return;
		}
		uart_write_bytes(UART_NUM_0, title+i, 1);

	}
}

void InitMonitorUart(){
	// / Configure parameters of an UART driver,
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};


	uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE, 0, NULL, 0);
	uart_write_bytes(UART_NUM_0, "UART0 begin...\n", 16);
    
}




// }

// void Logger::SetStyle(EnumStyle style_name, EnumStyle_position style_position){

//     if (style_name == EnumStyle::Debug){
//         if (style_position == EnumStyle_position::Title_Begin){
//             Serial.print(FORE_YELLOW);
//             Serial.print(BGC_GREEN);
//             Serial.print("[Debug] ");
//             return;
//         }else if (style_position == EnumStyle_position::Window_Begin){
//             Serial.print(BGC_BLACK);
//             Serial.println(FORE_GREEN);
//             return;
//         }
//     }

//     if (style_name == EnumStyle::Info) {
//         if (style_position == EnumStyle_position::Title_Begin){
//             Serial.print(FORE_YELLOW);
//             Serial.print(BGC_CYAN);
//             Serial.print("[Info] ");
//             return;
//         }else if (style_position == EnumStyle_position::Window_Begin){
//             Serial.print(BGC_BLACK);
//             Serial.println(FORE_CYAN);
//             return;
//         }
//     }


//     if (style_name == EnumStyle::Warning){
//         if (style_position == EnumStyle_position::Title_Begin){
//             Serial.print(FORE_BLUE);
//             Serial.print(BGC_YELLLOW);
//             Serial.print("[Warn] ");
//             return;
//         }else if (style_position == EnumStyle_position::Window_Begin){
//             Serial.print(BGC_BLACK);
//             Serial.println(FORE_YELLOW);
//             return;
//         }
//     }
//     if (style_name == EnumStyle::Error){
//         if (style_position == EnumStyle_position::Title_Begin){
//             Serial.print(FORE_BLUE);
//             Serial.print(BGC_RED);
//             Serial.print("[Error] ");
//             return;
//         }else if (style_position == EnumStyle_position::Window_Begin){
//             Serial.print(BGC_BLACK);
//             Serial.println(FORE_RED);
//             return;
//         }
//     }    
// }

// int Logger::sid = 0;

// void Logger::Info(const char* title){
//     // TRACE();
//     Logger::SetStyle(EnumStyle::Info, EnumStyle_position::Title_Begin);
//     Serial.print(title);
//     Logger::SetStyle(EnumStyle::Info, EnumStyle_position::Window_Begin);

// }
// // template <typename anyPrintable>
// // void Logger::Info(const char* title, anyPrintable var, anyPrintable value){
// //     Serial.print(FORE_YELLOW);
// //     Serial.print(BGC_BLUE);
// //     Serial.print("[Info] ");
// //     Serial.print(title);
// //     Serial.print("  ");
// //     Serial.print(value);
// //     Serial.print(BGC_BLACK);
// //     Serial.println(FORE_GREEN);
// // }

// void Logger::Debug(const char* title){
//     Logger::SetStyle(EnumStyle::Debug, EnumStyle_position::Title_Begin);
//     Serial.print(title);
//     Logger::SetStyle(EnumStyle::Debug, EnumStyle_position::Window_Begin);
//     // Serial.print(FORE_YELLOW);
//     // Serial.print(BGC_BLUE);
//     // Serial.print(title);
//     // Serial.print(BGC_BLACK);
//     // Serial.println(FORE_PINK);
// }

// void Logger::Warn(const char * title){
//     Logger::SetStyle(EnumStyle::Warning, EnumStyle_position::Title_Begin);
//     Serial.print(title);
//     Logger::SetStyle(EnumStyle::Warning, EnumStyle_position::Window_Begin);
// }

// void Logger::Error(const char * title){
//     Logger::SetStyle(EnumStyle::Error, EnumStyle_position::Title_Begin);
//     Serial.print(title);
//     Logger::SetStyle(EnumStyle::Error, EnumStyle_position::Window_Begin);
// }

// void Logger::Halt(const char* title){
//     Serial.print(FORE_DARK_GRAY);
//     Serial.print(BGC_BLACK);
//     Serial.print("[Halt]");
//     Serial.print(title);
//     Serial.print(BGC_YELLLOW);
//     // esp_random()
//     while (true){
//         Serial.print("\t I am playing my game.  So funny...   So boring... ");
//         delay(2000);
//     }
// }



// void Logger::__print_sid(){
//     Serial.print("("); 
//     Serial.print(Logger::sid); 
//     Serial.print(")"); 
//     Serial.print("\t");
//     Logger::sid++;

// }

// void Logger::Print(const char* var_name, const bool value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print("=\t");
//     Serial.println(value);
// }

// void Logger::Print(const char* var_name, const float value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print("=\t");
//     Serial.println(value);
// }

// void Logger::Print(const char* var_name, const double value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print("=\t");
//     Serial.println(value);
// }

// void Logger::Print(const char* var_name, const int32_t value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print(" =\t");
//     Serial.println(value);
// }

// void Logger::Print(const char* var_name, const int64_t value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print(" =\t");
//     Serial.println(value);
// }

// void Logger::Print(const char* var_name, const uint32_t value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print(" =\t");
//     Serial.println(value);
// }

// void Logger::Print(const char* var_name, const unsigned long value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print(" =\t");
//     Serial.println(value);
// }
// void Logger::Print(const char* var_name, const long value){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print(" =\t");
//     Serial.println(value);
// }


// void Logger::Print(const char* var_name, const char single_char){
//     __print_sid();
//     Serial.print(var_name);
//     Serial.print(" =\t");
//     Serial.println(single_char);
// }

// void Logger::Print(const char* key, const char* chars){
//     __print_sid();
//     Serial.print(key);
//     Serial.print("=\t");
//     Serial.println(chars);   // will print a string.
//     // Serial.println(*chars);  // will print single char.

// }

