#pragma once

#include <MFRC522.h>

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
        int NextStationType;  //todo: type is enum StationTypeOnMap.
        void Init(int8_t pin_clk, int8_t pin_miso, int8_t pin_mosi);
        // void LinkCallback(void(* callback)(uint16_t card_id)){this->__onDetectCard = callback;};
        bool ReadCard();

    private:
        MFRC522* __rfidReader;   // = MFRC522(10, 9);  // Create MFRC522 instance
        // void(* __onDetectCard)(uint16_t card_id);


};