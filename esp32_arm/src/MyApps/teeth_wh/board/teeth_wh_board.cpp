#include "teeth_wh_board.h"


#define PIN_HOMER_SENSOR_HALL_0 22
#define PIN_HOMER_SENSOR_HALL_1 22
#define PIN_HOMER_SENSOR_HALL_2 22
#define PIN_VACUUME_PUMP 22
#define PIN_VACUUME_SUCKER 22
#define PIN_HX711_CLK 22
#define PIN_HX711_DATA 22
#define PIN_SERVO 22



void TeethWarehouse_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am Teeth Warehouse.");
    }

    __all_position_triggers[0].Init(PIN_HOMER_SENSOR_HALL_0, LOW);
    __all_position_triggers[1].Init(PIN_HOMER_SENSOR_HALL_1, LOW);
    __all_position_triggers[2].Init(PIN_HOMER_SENSOR_HALL_2, LOW);
    HomeTrigger_Array::Instance().Init(__all_position_triggers, 4);
    
    pinMode(PIN_VACUUME_PUMP, OUTPUT);
    pinMode(PIN_VACUUME_SUCKER, OUTPUT);


    // Init servo.
    ESP32PWM::allocateTimer(0);   //https://github.com/madhephaestus/ESP32Servo/blob/master/examples/Multiple-Servo-Example-Arduino/Multiple-Servo-Example-Arduino.ino
    __servo.setPeriodHertz(50);      // Standard 50hz servo
    __servo.attach(PIN_SERVO);

    // Init Hx711
    __hx711.begin(PIN_HX711_DATA, PIN_HX711_CLK);

    // Init VL6180
    // Wire.begin();         // Start I2C library
    // delay(100);           // delay .1s



    // if (sensor.VL6180xInit() != 0)
    // {
    //     Serial.println("Failed to initialize. Freezing..."); // Initialize device and check for errors
    //     while (1);
    // }

    // sensor.VL6180xDefautSettings(); // Load default settings to get started.

    // delay(1000); // delay 1s

}




void TeethWarehouse_Board::EnableVacuumePump(bool enable_it){
    digitalWrite(PIN_VACUUME_PUMP, enable_it);
}

void TeethWarehouse_Board::EnableVacuumeSucker(bool enable_it){
    digitalWrite(PIN_VACUUME_SUCKER, enable_it);

}



void TeethWarehouse_Board::Test_PositionTriggers(int loops){
    uint32_t flags = 0;
    uint32_t last_flags = 999;
    int count =0;
    while (count < loops){
        if (flags != last_flags){
            Serial.print("Trigger is channged: \t\t");
            Serial.println(flags,BIN);
            last_flags = flags;
            count++;
        }
    }
}

void TeethWarehouse_Board::Test_Hx711(int loops){

    __hx711.set_scale(2280.f);          // this value is obtained by calibrating the scale with known weights; see the README for details
    __hx711.tare();				        // reset the scale to 0

    Serial.print("one reading:\t");
    Serial.print(__hx711.get_units(), 1);
    Serial.print("\t| average:\t");
    Serial.println(__hx711.get_units(10), 1);

    __hx711.power_down();			        // put the ADC in sleep mode
    delay(5000);
    __hx711.power_up();
}

void TeethWarehouse_Board::Test_Servo(int loops){
    int pos = 0;      // position in degrees
    for (pos = 0; pos <= 270; pos += 1) { // sweep from 0 degrees to 180 degrees
		// in steps of 1 degree
		__servo.write(pos);
		delay(1);             // waits 20ms for the servo to reach the position
	}
}

void TeethWarehouse_Board::Test_VL6180x(){
    // VL6180xIdentification identification;
    // sensor.getIdentification(&identification); // Retrieve manufacture info from device memory
    // // printIdentification(&identification);      // Helper function to print all the Module information
    // Serial.print("Model ID = ");
    // Serial.println(identification->idModel);

    // Serial.print("Model Rev = ");
    // Serial.print(identification->idModelRevMajor);
    // Serial.print(".");
    // Serial.println(identification->idModelRevMinor);

    // Serial.print("Module Rev = ");
    // Serial.print(identification->idModuleRevMajor);
    // Serial.print(".");
    // Serial.println(identification->idModuleRevMinor);

    // Serial.print("Manufacture Date = ");
    // Serial.print((identification->idDate >> 3) & 0x001F);
    // Serial.print("/");
    // Serial.print((identification->idDate >> 8) & 0x000F);
    // Serial.print("/1");
    // Serial.print((identification->idDate >> 12) & 0x000F);
    // Serial.print(" Phase: ");
    // Serial.println(identification->idDate & 0x0007);

    // Serial.print("Manufacture Time (s)= ");
    // Serial.println(identification->idTime * 2);
    // Serial.println();
    // Serial.println();
}
