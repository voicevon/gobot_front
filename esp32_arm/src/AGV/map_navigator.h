#pragma once
#include <stdint.h>

class MapSite{
    public:
        MapSite(){
            this->SiteId = 0;
        }
        enum TASK{
            FOLLOW_LEFT,
            FOLLLOW_RIGHT,
            LOADING,
            UNLOADING,
            SLEEPING,
            CHARGING,
        };
        MapSite(uint16_t site_id, TASK task ){
            this->SiteId = site_id;
            this->task = task;
            // this->FollowLeft = follow_left;
            // this->ShouldPark = should_park;
            };
        uint16_t SiteId; 
        TASK task;
        // bool FollowLeft = true;


        // bool ShouldPark = false;
};


class MapNavigator{
    public:
        MapNavigator();
        bool AddSite(uint16_t site_id, MapSite::TASK task);
        bool RemoveSite(uint16_t site_id);
        bool FetchSite(uint16_t site_id, MapSite* the_site );

    private:
        MapSite __all_sites[20];
};