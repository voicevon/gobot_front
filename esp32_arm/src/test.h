class Test{
    public:
        typedef enum
        {
            A  = 0, 
            B  = 1, 
        } MyEnum;
        Test(Test::MyEnum xx){};
};
class TT{
    private:
        int a;
    public:
        TT(){ 
            a= 5;
        };
};


class Runner{

    public:
         TT t2();
        Test* tp;
        Runner(){
            Test test(Test::MyEnum::A);
            tp = &test;
    }
    // >>> Compiled OK.  


        // Test test(Test::MyEnum::A);
        // // >>> src\test.h:15:33: error: 'Test::MyEnum::A' is not a type
        
        // Test test(Test::MyEnum.A);
        // // >>> src\test.h:19:31: error: expected ',' or '...' before '.' token
        
        // Test test(Test::MyEnum->A);
        // // >>> src\test.h:24:31: error: expected ',' or '...' before '->' token
        
        // Test test(Test::A);
        // // >>>  src\test.h:18:25: error: 'Test::A' is not a type


};