class Human{
    public:
        void Grow();
};
class Man:public Human{

};

class Dog{
    public:
        void Follow(Human* human){};
};

class test{
    Man xuming=Man();
    Dog tom=Dog();
    void gogo(){
        tom.Follow(&xuming);
    }
};

