#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_NODE

#include "Mesh/Router/adhoc_router.h"
#define PIN_YUNXING 27  //D5
#define PIN_TINGZHI 26  //D6
#define PIN_GUZHANG 25  //D7


AdhocRouter router;




void setup(){
  pinMode(PIN_YUNXING, INPUT_PULLUP);
  pinMode(PIN_TINGZHI, INPUT_PULLUP);
  pinMode(PIN_GUZHANG, INPUT_PULLUP);
}

void loop(){
    router.SpinOnce();
    //       myData.station_id = MY_ID;
    //   myData.io_1 = digitalRead(PIN_YUNXING);
    //   myData.io_2 = digitalRead(PIN_TINGZHI);
    //   myData.io_3 = digitalRead(PIN_GUZHANG);
}


#endif