#include "SingleAxisBLE.h"


SingleAxisBLE::SingleAxisBLE(){
    this->AppendGattChar(1,5);
}

int16_t SingleAxisBLE::GetTargetPositionAbs(){

}
void SingleAxisBLE::UpdateCurrentPos(int16_t value){

}
