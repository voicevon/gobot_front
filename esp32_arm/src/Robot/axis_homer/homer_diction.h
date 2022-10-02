// #pragma once

// #include "axis_homer.h"
// #include "CNC/coordinate/cnc_axis.h"
// #include "MyLibs/common_diction/diction_base.h"


// class HomerDiction: public DictionBase{
//     public:
//         static HomerDiction& Instance(){
//             static HomerDiction __instance;
//             return __instance;
//         }
//         void Init(AxisHomer* homers, int homers_count){_Init(homers, homers_count, sizeof(AxisHomer));};
//         AxisHomer* GetAxisHomer(EnumAxis_ForwardKinematic axis);
//         AxisHomer* GetAxisHomer(EnumAxis_Inverseinematic axis);


//     private:
//         AxisHomer __all_homers[CNC_AXIS_COUNT];  //todo: remove this.
//         uint8_t __axis_count;

// };