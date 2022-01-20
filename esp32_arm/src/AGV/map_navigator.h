#pragma once
#include <stdint.h>

class MapSite{
    public:
        MapSite(){
            this->SiteId = 0;
            this->FollowLeft = false;
            this->ShouldPark = true;
        }
        MapSite(uint16_t site_id, bool follow_left, bool should_park){
            this->SiteId= site_id;
            this->FollowLeft = follow_left;
            this->ShouldPark = should_park;
            };
        uint16_t SiteId; 
        bool FollowLeft = true;
        bool ShouldPark = false;
};


class MapNavigator{
    public:
        MapNavigator();
        bool AddSite(uint16_t site_id, bool follow_left, bool should_park);
        bool RemoveSite(uint16_t site_id);
        bool FetchSite(uint16_t site_id, MapSite* the_site );

    private:
        MapSite __all_sites[20];
};