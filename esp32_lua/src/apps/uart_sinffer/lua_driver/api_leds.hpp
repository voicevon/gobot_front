#pragma once

extern "C"{

    extern void InitCallback_CppSetLedState(void(*cpp_service_function)(int led_index,int state));
}
