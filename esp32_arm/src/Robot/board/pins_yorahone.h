#pragma once

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3
//  x-end   GPIO 17   x-step GPIO-12  x-dir GPIO-14  
//  y-end   GPIO 4    y-step GPIO-26  y-dir GPIO-15
//                    z-step GPIO-27  z-dir GPIO-33

#define PIN_ALPHA_DIR 14 //32    
#define PIN_ALPHA_STEP 12 //26   
#define PIN_ALPHA_ENABLE 13


#define PIN_GAMMA_DIR 15 //32  
#define PIN_GAMMA_STEP 26 //26   
#define PIN_GAMMA_ENABLE 13

#define PIN_BETA_DIR 33 //32  
#define PIN_BETA_STEP 14 //27 //26   
#define PIN_BETA_ENABLE 13


#define PIN_STATE_LED 16