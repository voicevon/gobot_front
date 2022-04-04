#pragma once

#include <MFRC522.h>
#include "AGV/map_road_station/map_road_station.h"
/*
    There are appplication information inside RFID, after we formated it.
    Those data are:
        1.  Map point id.  (Very foundimental data,  the other data is depending on this one.)
        2.  Next branch is (or not) on left side.
        3.  Next branch is workstation (or charge station, or park lot)
*/


class SmartRfidReader{
    public:
        int CardId;
        bool MainRoad_IsOn_LeftSide;
        WorkStation::TYPE_ON_MAP NextStationType;  
        void Init(int8_t pin_clk, int8_t pin_miso, int8_t pin_mosi);
        // void LinkCallback(void(* callback)(uint16_t card_id)){this->__onDetectCard = callback;};
        bool ReadCard();

    private:
        MFRC522* __rfidReader;
        // void(* __onDetectCard)(uint16_t card_id);


};