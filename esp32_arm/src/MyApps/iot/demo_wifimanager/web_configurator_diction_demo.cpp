#include "web_configurator_diction_demo.h"

WebConfiguratorDiction_Demo::WebConfiguratorDiction_Demo(){
    // config_items[0] = config_item_ssid;
    // config_items[1] = config_item_pass;
    // config_items[2] = config_item_admin_uid;
    // config_items[3] = config_item_admin_password;
    config_items[0] = &__ssid;
    config_items[1] = &__pass;
    config_items[2] = &__admin_uid;
    config_items[3] = &__admin_password;
    config_items[4] = &test_para;
    
    _items = config_items[0];
}

// bool WebConfiguratorDiction_Demo::HasItem(const char* item_name){
//     for(int i=0; i<4; i++){
//         int mismatch_at  = MemoryHelper::Find_MismachLocation(config_items[i], item_name, 30);
//         // int mismatch_at  = MemoryHelper::Find_MismachLocation(config_items[i], item_name, size_of(config_items[i]));
//         if (mismatch_at < 0)
//             return true;
//     }
//     return false;
// }