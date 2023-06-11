#include "MyLibs/utility/logger.h"
#include "ap_webserver_diction_base.h"

Html_Parameter::Html_Parameter(){
    __fs= &SPIFFS;
    __output_c_string.Init(__output_buffer, 30);
}

void Html_Parameter::SetName(const char* parameter_name){
    // Logger::Debug("Html_Parameter::SetName");
    // Logger::Print("parameter_name", parameter_name);
    for(int i=0; i<20; i++){
        char ch = *(parameter_name + i);
        __parameter_name[i] = ch;
        // Serial.print(char(xx));   // DOES NOT WORK ???
        if (ch == 0x00){
            return;
        }
    }
    Logger::Error("Html_Parameter::SetName()  Not found 0x00");
    Logger::Halt("");
}

const char* Html_Parameter::__GetSpiffsFilename(){
    char ch;
    __output_buffer[0] = '/';
    for(int i=0; i< 20; i++){
        ch = __parameter_name[i];
        __output_buffer[i+1] = ch;
        if (ch == 0x00){
            return &__output_buffer[0];
        }
    }
    Logger::Error("Html_Parameter::__GetSpiffsFilename()");
    Logger::Halt("");
    return __output_buffer;
};

void Html_Parameter::WriteToFile(AsyncWebParameter* p){
    const char * message = p->value().c_str();

    const char * path = this->__GetSpiffsFilename();
	Serial.printf("Writing file: %s\r\n", path);

	File file = __fs->open(path, FILE_WRITE);
	if(!file){
		Serial.println("- failed to open file for writing");
		return;
	}
	if(file.print(message)){
		Serial.println("- file written");
	} else {
		Serial.println("- write failed");
	}
}

bool Html_Parameter::IsMyName(const char * the_name){
    for(int i=0; i< 30; i++){
        char xx = *(the_name + i);
        if (xx != __parameter_name[i]){
            return false;
        }
        if (xx == 0x00){
            return true;
        }
    }
    Logger::Error("Html_Parameter::IsMyName()");
    Logger::Print("parameter", the_name);
    Logger::Halt("");
}

C_String_Base* Html_Parameter::ReadFile(){
    this->readFile();
    return &__output_c_string;
}

const char* Html_Parameter::readFile(){
    // Logger::Debug("Html_Parameter::readFile()");
    const char * path = this->__GetSpiffsFilename();
	// Serial.printf("Html_Parameter::readFile() : %s\r\n", path);

	File file = __fs->open(path);
	if(!file || file.isDirectory()){
		Logger::Print(" Html_Parameter::readFile()- failed to open file for reading--->", path);
        __output_buffer[0] = 0x00;
		return __output_buffer;
	}
	
	String fileContent;
	while(file.available()){
		fileContent = file.readStringUntil('\n');
		break;     
	}
    fileContent.toCharArray(__output_buffer, sizeof(fileContent));
    // Logger::Debug("Html_Parameter::readFile()");
    // Logger::Print(path, fileContent);
	return __output_buffer;
}


