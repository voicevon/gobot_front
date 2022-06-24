
class FkPositionBase{
    
};

class FkPosition_X: public FkPositionBase{
    public:
        float x;
};

class FkPosition_XY: public FkPositionBase{
    public:
        float X;
        float Y;
};

class FkPosition_XYZ: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
};

class FkPosition_XYZA: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
};

class FkPosition_XYZAB: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
};

class FPosition_XYZABC: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
        float c;
};

class FkPosition_ZW: public FkPositionBase{
    public:
        float Z;
        float W;
};

class FkPosition_YZ: public FkPositionBase{
    public:
        float Y;
        float Z;
};

class FkPosition_A: public FkPositionBase{
    public:
        float A;
};

class IkPositionBase{

};

// class FkPosition_AB: public FkPositionBase{
//     public:
//         float alpha;
//         float beta;
// };

class IkPosition_A: public IkPositionBase{
    public:
        float alpha;
};
class IkPosition_AB: public IkPositionBase{
    public:
        float alpha;
        float beta;
};