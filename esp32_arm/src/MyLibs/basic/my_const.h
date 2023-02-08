#pragma once
#include <stdint.h>

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define PAYLOAD_STRING_ON "ON"
#define PAYLOAD_STRING_OFF "OFF"

#define FCBC_RESET  "\033[0m"   //FCBC is for "Fore color background color"
// #define ctl_bold \033[01m
// #define ctl_disable='\033[02m'
// #define ctl_underline='\033[04m'
// #define ctl_reverse='\033[07m'
// #define ctl_strikethrough='\033[09m'
// #define ctl_invisible='\033[08m'

 
        // black='\033[30m'
#define FORE_RED "\033[31m"
#define FORE_GREEN "\033[32m"
#define FORE_ORANGE "\033[33m"
#define FORE_BLUE "\033[34m"
    //     purple='\033[35m'
    //     cyan='\033[36m'
#define FORE_LIGHT_GRAY "\033[37m"
#define FORE_DARK_GRAY "\033[90m"
    //     lightred='\033[91m'
    //     lightgreen='\033[92m'
#define FORE_YELLOW "\033[93m"
    //     lightblue='\033[94m'
#define  FORE_PINK "\033[95m"
    //     lightcyan='\033[96m'

    // class Background: 
    //     '''
    //     Full name: Perfect_background_color
    //     ''' 
#define BGC_BLACK "\033[40m"
#define BGC_RED "\033[41m"
    //     green='\033[42m'
#define BGC_ORANGE "\033[43m"
#define BGC_BLUE  "\033[44m"
    //     purple='\033[45m'
    //     cyan='\033[46m'
    //     lightgrey='\033[47m'

