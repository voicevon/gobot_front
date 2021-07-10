#include "house.h"
#include <Wire.h>

#define PIN_LEFT_SERVO 4
#define PIN_RIGHT_SERVO 5

#define PIN_I2C_SCLK 23
#define PIN_I2C_DATA 22

#define ENDER_COIL 24

House::House()
{
}

void House::SpinOnce()
{
}

void House::DrawStone(uint8_t house_id)
{
    // Bottom mover : move to original position
    __LeftServo.write(0);
    __RightServo.write(180);

    // Enable a coil to stick a stone
    EnableSingleCoil(house_id, true);

    // Bottom mover:  Leave original position
    __LeftServo.write(180);
    __RightServo.write(0);

    // Stop the coil
    EnableSingleCoil(house_id, false);
}

void House::Scanner()
{
    Serial.println();
    Serial.println("I2C scanner. Scanning ...");
    byte count = 0;

    Wire.begin();
    for (byte i = 8; i < 120; i++)
    {
        Wire.beginTransmission(i);       // Begin I2C transmission Address (i)
        if (Wire.endTransmission() == 0) // Receive 0 = success (ACK response)
        {
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX); // PCF8574 7 bit address
            Serial.println(")");
            count++;
        }
    }
    Serial.print("Found ");
    Serial.print(count, DEC); // numbers of devices
    Serial.println(" device(s).");
}

void House::Setup(RobotAction *pAction)
{
    // __Mcp23018 = &Mcp23018::getInstance();
    // __Mcp23018->DisableAllCoils();
    __LeftServo.attach(PIN_LEFT_SERVO);
    __RightServo.attach(PIN_RIGHT_SERVO);

    __house_action = pAction;
    __current_coil_index = 0;
    {

        // Wire.begin(PIN_I2C_DATA, PIN_I2C_SCLK, 400000);
        /*  The logic coil_id count is 52.
        The idea is 
            1. let logic_coil_id as a key.
            2. We can get chip_I2C_addr, pin_index inside chip, and next logic id
        Unfortunitely, we can NOT define an const array with greater than 128 bytes
        So, we define 3 individual ditionaries.
    */

        // uint8_t COIL_TABLE[52*3] = {
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //0..4
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //5..9

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //10..14
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //15..19

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //20..24
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //25..29

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //30..34
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //35..39

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //40..44
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //45..49

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //50..54
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //55..59

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //60..64
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //65..69

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //70..74
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //75..79

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //80..84
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //85..89

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //90..94
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //95..99

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //100..104
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //105..109

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //110..104
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //115..109

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //120..104
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //125..109

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //130..104
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //135..109

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //140..104
        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //145..109

        //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //150..104
        //     0x34,2,2,                                                             //155
        //     }
    }
    // Below are three loolup tables
    {
        //Index is logic coil id, value is next coil logic id
        uint8_t next_coil_id[COIL_COUNT] = {
            2, 4, 5, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, //count 14
            28, 29,

            18, 3, 20, 7, 22, 11, 24,

            15, 26, 19, 28, 23, 25, 27,
            29, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
            40, 41, 42, 43, 44, 45, 46, 47, 48, 49, ENDER_COIL};
        // 51,52,53,53};    //Will never touch this coli. because it is NOT exist!
        memcpy(__NextCoilId, next_coil_id, sizeof(next_coil_id));

        //Index is logic coil id, value is I2c address,
        // uint8_t table_addr[COIL_COUNT] = {
        //     0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,0x48,  // count 14
        //     0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,  // count 14
        //     0X46,0x16,0x46,0x46,0x46,0X46,0x16,0x46,0x46,0x46,0x46,0x46,0x46,       // count 13
        //     0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40, 0x40,0x40,0x40,0x40            // count 12
        //     };
        //     // 0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42 // count 16
        //     // };
        // memcpy(table_addr, __I2cAddress, sizeof(table_addr));

        __I2cAddress[0] = 0x48;
        // __I2cAddress[1] = 0x4c;
        __I2cAddress[1] = 0x26;
        __I2cAddress[2] = 0x46;
        __I2cAddress[3] = 0x40;
        // __I2cAddress[4] = 0x00;

        uint8_t table_chip_index[COIL_COUNT] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //14
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //14
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,    //13
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,       //12
            //4,4,4,4,4,4,4,4                 //8

        };
        memcpy(__Chip_Index, table_chip_index, sizeof(table_chip_index));

#define MCP_A0 0
#define MCP_A1 1
#define MCP_A2 2
#define MCP_A3 3
#define MCP_A4 4
#define MCP_A5 5
#define MCP_A6 6
#define MCP_A7 7
#define MCP_B0 8
#define MCP_B1 9
#define MCP_B2 10
#define MCP_B3 11
#define MCP_B4 12
#define MCP_B5 13
#define MCP_B6 14
#define MCP_B7 15
        //Index is logic coil id, value is phsical coil id inside mcp23018
        uint8_t table_pin_index[COIL_COUNT] = {
            MCP_B6, MCP_B5, MCP_B4, MCP_B3, MCP_B2, MCP_B1, MCP_B0, MCP_A6, MCP_A5, MCP_A4, MCP_A3, MCP_A2, MCP_A1, MCP_A0, // count 14
            MCP_B6, MCP_A0,

            MCP_B0, MCP_A1, MCP_B1, MCP_A2, MCP_B2, MCP_A3, MCP_B3,

            MCP_A4, MCP_B4, MCP_A5, MCP_B5, MCP_B6,                                                                 // count 14
            MCP_A6, MCP_A5, MCP_A4, MCP_A3, MCP_A2, MCP_A1, MCP_A0, MCP_B0, MCP_B1, MCP_B2, MCP_B3, MCP_B4, MCP_B5, // count 13
            MCP_A0, MCP_A1, MCP_A2, MCP_A3, MCP_A4, MCP_A5, MCP_A6, MCP_B0, MCP_B1, MCP_B2, MCP_B3, MCP_B4          // count 12
        };
        // A0,A1,A2,A3,A4,A5,A6,A7,B0,B1,B2,B3,B4,B5,B6,B7
        // };
        memcpy(__Pin_in_chip, table_pin_index, sizeof(table_pin_index));
    }

    Wire.begin(PIN_I2C_DATA, PIN_I2C_SCLK, 400000);
    Scanner();
    uint8_t addr = 0;
    uint8_t last_addr = 0xff;
    uint chip_index = 0;
    // for (int i = 1; i < COIL_COUNT; i++)
    int i=1;
    {
        chip_index = __Chip_Index[i];
        chip_index = 1;
        addr = __I2cAddress[chip_index];
        // Serial.print(chip_index);
        // Serial.print("    >>>>");
        // Serial.print(addr);
        // Serial.print("----");
        // Serial.print(last_addr);
        // Serial.print("<<<<<<   ");
        if (addr != last_addr)
        {
            Serial.print("\n    Init MCP23018, Index, Addr   ");
            Serial.print(chip_index);
            Serial.print(".......");
            Serial.print(addr);
            __Mcp23018[chip_index] = new mcp23018(addr);
            // __Mcp23018[chip_index]->begin()
            __Mcp23018[chip_index]->gpioPinMode(OUTPUT);
            __Mcp23018[chip_index]->portPullup(HIGH);
            chip_index++;
            last_addr = addr;
        }
    }
    Serial.println("\nccccccccccccccccccccccccccccccccc");
    // MoveStoneToTarget(16);
    while (true){
        Test();
    }
    Serial.println("ffffffffffffffffffffffffffffffffffff");
}

uint8_t ppp[6];

void House::Test(){
    ppp [0] = 0;
    ppp [1] = 2;
    ppp [2] = 3;
    ppp [3] = 5;
    ppp [4] = 4;
    ppp [5] = 8;
    // uint8_t chip_index = 1
    // __Mcp23018[1]->begin();
    for(int i=0; i<5;i++){
        __Mcp23018[1]->gpioDigitalWrite(ppp[i], false);
    }
    for (int i=0; i<5; i++){
        Serial.print("     >>>>>  ");
        Serial.print(i);     
        Serial.print(" == ");
        Serial.print(ppp[i]);        

        Serial.print("  on");     
        __Mcp23018[1]->gpioDigitalWrite(ppp[i], true);
        __Mcp23018[1]->gpioDigitalWrite(ppp[i+1], true);
        // __Mcp23018[1]->gpioDigitalWrite(5, true);
        delay(2000);
        Serial.print("  off\n");        

        __Mcp23018[1]->gpioDigitalWrite(ppp[i], false);
        // __Mcp23018[1]->gpioDigitalWrite(5, false);
        delay(300);
    }
    Serial.print("  \n");        
    // while (true){}
}
void House::EnableSingleCoil(int logic_coil_id, bool enable_it)
{
    // Prepare the data value of the action.
    uint8_t chip_index = __Chip_Index[logic_coil_id];
    uint8_t pin_index = __Pin_in_chip[logic_coil_id];
    Serial.print("\n chip, pin ");
    Serial.print(chip_index);
    Serial.print("   ");
    Serial.print(pin_index);

    __Mcp23018[chip_index]->gpioDigitalWrite(pin_index, enable_it);

    last_enabled_logic_coil_id = logic_coil_id;
}

void House::MoveStoneToTarget(uint8_t start_coil)
{
    int last_coil = start_coil;
    // Assume all the coils are disable before invoking.
    while (last_coil != ENDER_COIL)
    {
        EnableSingleCoil(last_coil, true);
        delay(5000);
        EnableSingleCoil(last_coil, false);
        delay(5000);
        last_coil = __NextCoilId[last_coil];
    }
}
