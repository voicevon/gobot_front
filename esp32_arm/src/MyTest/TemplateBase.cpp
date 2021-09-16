#include "TemplateBase.h"


// http://www.cs.technion.ac.il/users/yechiel/CS/FAQs/c++-faq/nondependent-name-lookup-members.html


template <int nnnn>
void Base<nnnn>::bget(int x){
    // return N;
}

Derived::Derived(){

}
void Derived::dget(){
    this->bget(6);
}

Derived d;

int main(){
    d.bget(9); 
    d.dget();
}