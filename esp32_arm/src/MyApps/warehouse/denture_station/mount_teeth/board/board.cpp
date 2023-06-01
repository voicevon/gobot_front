#include "board.h"





// valve::output h_bridge
#define PIN_H_BRIDGE_PWM_SPEED 23
#define PIN_H_BRIDGE_DIR 24
// valve::indicator
#define PIN_LED_GO_STRAIGHT_BLUE 17
#define PIN_LED_GO_STRAIGHT_RED 18
#define PIN_LED_GO_STRAIGHT_GREEN 19
#define PIN_LED_TURN_RIGHT_BLUE 20
#define PIN_LED_TURN_RIGHT_RED 33
#define PIN_LED_TURN_RIGHT_GREEN 31

// valve::input ir-reflactor
#define PIN_GO_STRAIGHT_STOPPER 21
#define PIN_TURN_RIGHT_STOPPER 22

// valve::input ir-blocking
#define PIN_INLET_IR_SENSOR 27
#define PIN_OUTLET__IR_SENSOR_GO_STRAIGHT 28
#define PIN_OUTLET_IR_SENSOR_TURN_RIGHT 29



// barcode reader
#define PIN_BARCODE_READER_RX 33
#define PIN_BARCODE_READER_TX 34

// digital_led:: output
#define PIN_DIGITAL_LED_QUAD_RESET 25
#define PIN_DIGITAL_LED_QUAD_DATA 25
#define PIN_DIGITAL_LED_QUAD_CLOCK 25

#define PIN_DEBUG_LED 2




void ThreeWayValveBoard::Init(){
    Serial.begin(115200);
    
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");
    delay(1000);

    // __barcode_reader.Init(PIN_BARCODE_READER_RX, PIN_BARCODE_READER_TX);
    // __digital_number.Init(PIN_DIGITAL_LED_QUAD_CLOCK, PIN_DIGITAL_LED_QUAD_DATA, PIN_DIGITAL_LED_QUAD_RESET,4);

    __go_straight_stopper.Init('T', PIN_GO_STRAIGHT_STOPPER, HIGH);
    // __turn_right_stopper.Init('L', PIN_TURN_RIGHT_STOPPER, HIGH);

    // __h_bridge.Init(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_PWM_SPEED);
    // __dc_motor.LinkHBridgeDriver(&__h_bridge);

    // __reciprocator.LinkActuator(&__dc_motor);
    // __reciprocator.LinkPositionTriger(&__go_straight_stopper, &__turn_right_stopper);
    // __valve.LinkReciprocator(&__reciprocator);

    // __inlet_ir_sensor.Init('I', PIN_INLET_IR_SENSOR, HIGH);
    // __outlet_ir_sensor_go_straight.Init('T', PIN_OUTLET__IR_SENSOR_GO_STRAIGHT, HIGH);
    // __outlet_ir_sensor_turn_right.Init('L', PIN_OUTLET_IR_SENSOR_TURN_RIGHT, HIGH);
    // __valve.LinkIrSensors(&__inlet_ir_sensor, &__outlet_ir_sensor_go_straight, &__outlet_ir_sensor_turn_right);

    // __led_go_straight_blue.Init('B', PIN_LED_GO_STRAIGHT_BLUE, HIGH);
    // __led_go_straight_red.Init('R', PIN_LED_GO_STRAIGHT_RED, HIGH);
    // __led_go_straight_green.Init('G', PIN_LED_GO_STRAIGHT_GREEN, HIGH);
    // __led_turn_right_blue.Init('b', PIN_LED_TURN_RIGHT_BLUE, HIGH);
    // __led_turn_right_red.Init('r', PIN_LED_TURN_RIGHT_RED, HIGH);
    // __led_turn_right_green.Init('g', PIN_LED_TURN_RIGHT_GREEN, HIGH);

    // __valve.LinkLeds(&__led_go_straight_green, 
    //                 &__led_go_straight_red,
    //                 &__led_go_straight_blue,
    //                 &__led_turn_right_green,
    //                 &__led_turn_right_red,
    //                 &__led_turn_right_blue );

    __led_debug.Init('D', PIN_DEBUG_LED, HIGH);
    __led_debug.TurnOn();
}


void ThreeWayValveBoard::TestPositionTriggers(int loops){
    while (true){
        bool x = digitalRead(21);
        digitalWrite(2, x);
    }
    // static bool go_straight_is_fired;
    // static bool turn_right_is_fired;
    // int loop_count = 0;
    // while (loop_count < loops){
    //     if (__go_straight_stopper.IsFired()){
    //         __led_debug.TurnOn();
    //         if(!go_straight_is_fired){
    //             Logger::Print("__go_straight_stopper", "Fired");
    //             go_straight_is_fired = true;
    //         }
    //         loop_count++;
    //     }else {
    //         __led_debug.TurnOff();
    //         if (go_straight_is_fired){
    //         Logger::Print("__go_straight_stopper", "muted");
    //         go_straight_is_fired = false;
    //         }
    //     }

    //     // if (__turn_right_stopper.IsFired()){
    //     //     Logger::Print("__turn_right_stopper", "Fired");
    //     //     turn_right_is_fired = true;
    //     //     __led_debug.TurnOn();
    //     //     loop_count++;
    //     // }else if (turn_right_is_fired){
    //     //     Logger::Print("__go_straight_stopper", "muted");
    //     //     turn_right_is_fired = false;
    //     //     __led_debug.TurnOff();
    //     // }
    // }    

}
