#include "teeth_wh_eef.h"


void TeethWarehouse_RobotEef::Run(uint8_t eef_code){
    switch (eef_code){
        case EEF_CODE_UP:
            /* code */
            break;
        case EEF_CODE_DOWN:
            
            break;;
        case EEF_CODE_SUCK:
        
            break;;
        case EEF_CODE_RELEASE:
        
            break;;
        case EEF_CODE_ENABLE_VACUUME:
            this->board->EnableVacuumPump(true);
            break;;
        case EEF_CODE_DISABLE_VACUUME:
            this->board->EnableVacuumPump(false);
            break;;
        default:
            break;
    }
}

