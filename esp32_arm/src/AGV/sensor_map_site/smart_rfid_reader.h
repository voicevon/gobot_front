#pragma once

#include "AGV/map_road_station/map_road_station.h"
#include "map_site_reader.h"
#include <MFRC522.h>
/*
    There are appplication information inside RFID, after we formated it.
    Those data are:
        1.  Map point id.  (Very foundimental data,  the other data is depending on this one.)
        2.  Next branch is (or not) on left side.
        3.  Next branch is workstation (or charge station, or park lot)
*/


class SmartRfidReader: public MapSiteReaderBase{
    public:
        SmartRfidReader(){};
        int CardId;
        bool MainRoad_IsOn_LeftSide;
        WorkStation::TYPE_ON_MAP NextStationType;  
        
        void LinkDriver(MFRC522* reader){this->__mfrc522=reader;};
        bool ReadCard();
        int ReadMapSiteId() override;

    private:
        MFRC522* __mfrc522;

};