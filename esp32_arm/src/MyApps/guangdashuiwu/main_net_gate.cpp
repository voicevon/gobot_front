#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_NETGATE

#include "Mesh/Router/adhoc_router.h"

#define PIN_YUNXING_4 13
#define PIN_TINGZHI_4 32
#define PIN_GUZHANG_4 4
#define PIN_YUNXING_2 25
#define PIN_TINGZHI_2 27
#define PIN_GUZHANG_2 14

AdhocRouter router;

void setup(){
  pinMode(PIN_YUNXING_2, OUTPUT);
  pinMode(PIN_TINGZHI_2, OUTPUT);
  pinMode(PIN_GUZHANG_2, OUTPUT);
  pinMode(PIN_YUNXING_4, OUTPUT);
  pinMode(PIN_TINGZHI_4, OUTPUT);
  pinMode(PIN_GUZHANG_4, OUTPUT);
}

void loop(){
    router.SpinOnce();
        // case 4:
        digitalWrite(PIN_YUNXING_4, !rx_buffer.io_21);
        digitalWrite(PIN_TINGZHI_4, !rx_buffer.io_22);
        digitalWrite(PIN_GUZHANG_4, !rx_buffer.io_23);
    //     break;
    // case 2:
        digitalWrite(PIN_YUNXING_2, !rx_buffer.io_41);
        digitalWrite(PIN_TINGZHI_2, !rx_buffer.io_42);
        digitalWrite(PIN_GUZHANG_2, !rx_buffer.io_43);
}


#endif