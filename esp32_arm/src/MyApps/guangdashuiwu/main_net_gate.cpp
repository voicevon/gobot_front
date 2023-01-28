#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_NETGATE

#include "Mesh/Router/adhoc_router.h"
#include "board.h"



AdhocRouter router;
GuangDa_ShuiWu_Board board;

void setup(){
    board.Init();

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