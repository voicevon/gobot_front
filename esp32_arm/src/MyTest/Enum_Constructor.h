// class myClass
//   {
//   volatile bool switchChanged;

//   static myClass * instances [2];
 
//   static void switchPressedExt0 ()
//     {
//     if (myClass::instances [0] != NULL)
//       myClass::instances [0]->switchPressed ();
//     }  // end of myClass::switchPressedExt0
  
//   static void switchPressedExt1 ()
//     {
//     if (myClass::instances [1] != NULL)
//       myClass::instances [1]->switchPressed ();
//     }  // end of myClass::switchPressedExt1
  

//   public:

//   void begin (const byte whichPin)
//     {
//     pinMode (whichPin, INPUT_PULLUP);
//     switch (whichPin)
//       {
//       case 2: 
//         attachInterrupt (0, switchPressedExt0, CHANGE);
//         instances [0] = this;
//         break;
        
//       case 3: 
//         attachInterrupt (1, switchPressedExt1, CHANGE);
//         instances [1] = this;
//         break;
        
//       } // end of switch
//     }  // end of myClass::begin
    
//   void switchPressed ()
//     {
//     switchChanged = true; 
//     }
    
//   };  // end of class myClass
  
// myClass * myClass::instances [2] = { NULL, NULL };

// // instances of our class  
// myClass foo; 
// myClass bar;

// void setup ()
//   {
//   foo.begin (2);   // pin D2
//   bar.begin (3);   // pin D3
//   }  // end of setup

// void loop ()
//   {
//   // whatever    
//   }  // end of loop











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