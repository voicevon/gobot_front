#include "single_axis_homer.h"


class MultiPositionHomer{
    public:

        MultiPositionHomer(uint8_t triger_count);
        void LinkSingleHomer(SingleAxisHomer* single_homer);

        //return -1, if no homer is trigered.
        uint8_t GetTrigeredIndex();

    private:
        uint8_t __triger_count;
        SingleAxisHomer* _all_single_homers[4];


};
