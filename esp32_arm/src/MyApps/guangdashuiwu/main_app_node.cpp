#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_NODE

#include "Mesh/Router/adhoc_router.h"
#include "board.h"



AdhocRouter router;
GuangDa_ShuiWu_Board board;




void setup(){
    board.Init();

}

void loop(){
    router.SpinOnce();
    //       myData.station_id = MY_ID;
    //   myData.io_1 = digitalRead(PIN_YUNXING);
    //   myData.io_2 = digitalRead(PIN_TINGZHI);
    //   myData.io_3 = digitalRead(PIN_GUZHANG);
}


#endif