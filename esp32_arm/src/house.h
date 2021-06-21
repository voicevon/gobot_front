#pragma once

#include "actions.h"

class House{
    public:
        House();
        void Init(void);
        void SpinOnce(HouseAction action);
        void DrawStone(void);
    protected:
    private:
};