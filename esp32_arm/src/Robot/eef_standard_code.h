#pragma once

enum class EefAction{
    Lower = 1,
    Higher = 2, 
    // also Load
    Suck = 3,  
    Load = 3,
    // also Unload
    Release = 4,  
    Unload = 4,
    Sleep = 5,
};