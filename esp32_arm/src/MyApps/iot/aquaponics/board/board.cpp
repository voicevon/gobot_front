#include "board.h"

#define SDA_PIN 21
#define SCL_PIN 22
#define FREQ 0U

void AquaponicBoard::Init(){
    this->__ms5837.Init(&__my_i2c);
    this->__my_i2c.begin(SDA_PIN , SCL_PIN , FREQ);
}
