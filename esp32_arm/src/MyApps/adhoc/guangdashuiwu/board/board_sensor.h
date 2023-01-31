#pragma once
#include "MyLibs/basic/board_base.h"
#include "MyLibs/component/button_gpio.h"
#include "MyLibs/component/led_gpio.h"

#define PIN_ZIDONG 26  //D6
#define PIN_YUNXING 27  //D5
#define PIN_GUZHANG 25  //D7


class GuangDa_ShuiWu_SensorBoard: public BoardBase{
    public:
        void Init() override;
        void SpinOnce();
        uint8_t GetPayload();
        Led_GPIO* GetStateLed(){return &__state_led;};

    private:
        Button_Gpio __zidong_shoudong = Button_Gpio(PIN_ZIDONG);
        Button_Gpio __yunxing_tingzhi = Button_Gpio(PIN_YUNXING);
        Button_Gpio __guzhang_zhengchang = Button_Gpio(PIN_GUZHANG);
        Led_GPIO __state_led;
        

};