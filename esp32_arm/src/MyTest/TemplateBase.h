

template <int N>
class Base{
    public:
        void bget(int v);
};

class Derived:public Base<5>{
    public:
        Derived();
        void dget();
};

