#ifndef __MYLIBS_MYFUNCTIONS_HPP_
#define __MYLIBS_MYFUNCTIONS_HPP_

#include "HardwareSerial.h"

// using std::string;
template<class T>
void SerialPrintString(std::string str){
    Serial.println();
    Serial.println(str.c_str());
    for(const char* p=str.c_str(); *p; ) {
        Serial.print(*p++);
  }
}

#include <string>
#include <sstream>
template<class T>
std::string ToString(const T &value) {
    std::ostringstream os;
    os << value;
    return os.str();
}




class EmptyClass{};

#endif
