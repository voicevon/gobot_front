#pragma once


class MoveBlock_SingleActuator{
    public:
        // EnumAxis_Inverseinematic axis;
        bool IsAbsTargetPosition;  // Should be here?
        float TargetPosition;
        void Recalulate_distance_speed_acceleration(float start_position, float the_time);
        float VectorDistance;   //
        float Speed;
        float Acceleration;

};
