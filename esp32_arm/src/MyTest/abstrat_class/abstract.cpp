class base{
    public:
        void bbb();
};
class child:public base{

};

class user{
    public:
        void go(base* bb){};
};

class test{
    child cc=child();
    user uu=user();
    void gogo(){
        uu.go(&cc);
    }
};

