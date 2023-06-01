#include "MyLibs/basic/logger.h"
#include "web-configurator_parameter.h"

WebConnfigurator_Parameter::WebConnfigurator_Parameter(){

}

WebConnfigurator_Parameter::WebConnfigurator_Parameter(const char* item_name){
    SetName(item_name);
}

void WebConnfigurator_Parameter::SetName(const char* item_name){
    for(int i=0; i<20; i++){
        char xx = *(item_name + i);
        __item_name[i] = xx;
        if (xx == 0x00){
            return;
        }
    }
}

const char* WebConnfigurator_Parameter::GetSpiffsFilename(){
    __output_buffer[0] = '/';
    for(int i=0; i< 30; i++){
        char xx = __item_name[i];
        __output_buffer[i+1] = xx;
        if (xx = 0x00){
            return &__output_buffer[0];
        }
    }
};

void WebConnfigurator_Parameter::WriteToFile(AsyncWebParameter* p){
    // p->value().toCharArray(__ssid,MAX_STRING_LENGTH_IN_HTML_INPUT);
    writeFile(SPIFFS, p->value().c_str());
}

// void WebConnfigurator_Parameter::Link_AsyncWebParameter(){

// }

bool WebConnfigurator_Parameter::IsMyName(const char * the_name){
    for(int i=0; i< 30; i++){
        char xx = *(the_name + i);
        if (xx != __item_name[i]){
            return false;
        }
        if (xx == 0x00){
            return true;
        }
    }
    Logger::Error("WebConnfigurator_Parameter::IsMyName()");
    Logger::Print("parameter", the_name);
    Logger::Halt("");
}

// Read File from SPIFFS
const char* WebConnfigurator_Parameter::readFile(){
    fs:FS fs = SPIFFS;
    const char * path = this->GetSpiffsFilename();
	Serial.printf("Reading file: %s\r\n", path);

	File file = fs.open(path);
	if(!file || file.isDirectory()){
		Serial.println("- failed to open file for reading");
		return String().c_str();   // any risk?
	}
	
	String fileContent;
	while(file.available()){
		fileContent = file.readStringUntil('\n');
		break;     
	}
	return fileContent.c_str();  // any risk?
}

// Write file to SPIFFS
void WebConnfigurator_Parameter::writeFile(fs::FS &fs,  const char * message){
    const char * path = this->GetSpiffsFilename();
	Serial.printf("Writing file: %s\r\n", path);

	File file = fs.open(path, FILE_WRITE);
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
