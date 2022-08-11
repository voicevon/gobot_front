#include "board_test_base.h"

void BoardTestBase::Test_AllHomers(SingleAxisHomer* homer, int loop_count){
    for (int i=0; i<loop_count; i++){
        Serial.print("Homer is Triger = ");
        Serial.println(homer->IsTriged());
        delay(1000);
    }
}


