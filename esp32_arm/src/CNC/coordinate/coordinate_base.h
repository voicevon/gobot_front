#pragma once



class FKPosition_XYZRPY{
    public:
        float X;
        float Y;
        float Z;
        float Roll;  // about axis-X
        float Pitch;  // about axis-Y
        float Yaw;  // about axis-Z
        void PrintOut(const char* title);
};

class IKPosition_abgdekl{
    public:
        float Positions[7];
        void PrintOut(const char* title);
        // float alpha;
        // float beta;
        // float gamma;
        // float delta;
        // float eppsilon;  
        // float kappa; 
        // float lambda;
};