# include "map_navigator.h"

MapNavigator::MapNavigator(){
    for(int i=0; i<20; i++){
        this->__all_sites[i].SiteId = 0;
    }
}

bool MapNavigator::AddSite(uint16_t site_id, MapSite::TASK task){
    for(int i=0; i<20; i++){
        if (this->__all_sites[i].SiteId == 0){
            this->__all_sites[i].SiteId = site_id;
            this->__all_sites[i].task = task;
            // this->__all_sites[i].FollowLeft = follow_left;
            // this->__all_sites[i].ShouldPark = should_park;
            return true;
        }
    }
    return false;   
}

bool MapNavigator::RemoveSite(uint16_t site_id){
    for(int i=0; i<20; i++){
        if (this->__all_sites[i].SiteId == site_id){
            this->__all_sites[i].SiteId = 0;
            return true;
        }
    }
    return false;
}


bool MapNavigator::FetchSite(uint16_t site_id, MapSite* the_site ){
    return false;
}