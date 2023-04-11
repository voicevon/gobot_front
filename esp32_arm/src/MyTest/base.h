#pragma once

// class T{

// };
template <class T>
class queue{
    public:
        int say_hello(T* t){__t = t; return 123; };
    public:
        int _hello();
        // int _hello(){return 333;};
    private:
        T* __t;
};

// template<class T>
// int queue<T>::_hello(){
// // int queue::_hello(){
//     return 333;
// }


class line{
    public:
        void draw(){};
};

class lines_queue: public queue<line>{
    public:
        // static lines_queue& instance(){
        //     static lines_queue  __instance;
        //     return __instance;
        // };
        // int go() {return _hello();};
        lines_queue(){};
        
    private:
        // lines_queue(){};
};





// void jjjjh(){
//         line ll;
//     int a = lines_queue::instance().go();
//     int b = lines_queue::instance().say_hello(&ll);
//     int x = a+b;
// }