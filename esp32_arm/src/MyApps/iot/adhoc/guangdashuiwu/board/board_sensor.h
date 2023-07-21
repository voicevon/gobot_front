#pragma once
#include "Robot/board/board_base.h"
#include "button_gpio_ver2022.h"

#include "von/component/display/mono_led_gpio.h"


#define PIN_ZIDONG 26  //D6
#define PIN_YUNXING 27  //D5
#define PIN_GUZHANG 25  //D7


class GuangDa_ShuiWu_SensorBoard: public BoardBase{
    public:
        void Init(const char* app_welcome_statement) override;
        void SpinOnce();
        uint8_t GetPayload();
        Mono_Led_GPIO* GetStateLed(){return &__state_led;};

    private:
        
        Button_Gpio __zidong_shoudong = Button_Gpio(PIN_ZIDONG);
        Button_Gpio __yunxing_tingzhi = Button_Gpio(PIN_YUNXING);
        Button_Gpio __guzhang_zhengchang = Button_Gpio(PIN_GUZHANG);

        // If this project need to be reprojected, DO THESE:
        // BinaryInput_GPIO __zidong_shoudong = BinaryInput_GPIO(11, PIN_ZIDONG, HIGH);
        // BinaryInput_GPIO __yunxing_tingzhi = BinaryInput_GPIO(12, PIN_YUNXING, HIGH);
        // BinaryInput_GPIO __guzhang_zhengchang = BinaryInput_GPIO(13, PIN_GUZHANG, HIGH);
        Mono_Led_GPIO __state_led;
        

};