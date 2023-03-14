#pragma once


class MoveBlock_SingleActuator{
    public:
        // EnumAxis_Inverseinematic axis;
        float TargetPosition;
        float VectorDistance;   //
        float Speed;   // unit is mm/s
        float Acceleration; // unit is mm/(s*s)

};
