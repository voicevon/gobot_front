#include "all_applications.h"
#ifdef I_AM_TESTER

#include "base.h"

void setup(){

}

void loop(){
    line ll;
    // int a = lines_queue::instance().go();
    // int b = lines_queue::instance().say_hello(&ll);
    lines_queue lq;
    int a = lq._hello();
    int x = lq.say_hello(&ll);
}


#endif