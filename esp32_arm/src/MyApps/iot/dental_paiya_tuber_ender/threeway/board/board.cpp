#include "board.h"


// output led
#define PIN_LED_GO_STRAIGHT_BLUE 17
#define PIN_LED_GO_STRAIGHT_RED 18
#define PIN_LED_GO_STRAIGHT_GREEN 19
#define PIN_LED_TURN_RIGHT_BLUE 20
#define PIN_LED_TURN_RIGHT_RED 21
#define PIN_LED_TURN_RIGHT_GREEN 22
// output h_bridge
#define PIN_H_BRIDGE_PWM_SPEED 23
#define PIN_H_BRIDGE_DIR 24

// input ir-reflactor
#define PIN_GO_STRAIGHT_STOPPER 15
#define PIN_TURN_RIGHT_STOPPER 16
// input ir-blocking
#define PIN_INLET_IR_SENSOR 27
#define PIN_OUTLET__IR_SENSOR_GO_STRAIGHT 28
#define PIN_OUTLET_IR_SENSOR_TURN_RIGHT 29

// barcode reader
#define PIN_BARCODE_READER_RX 33
#define PIN_BARCODE_READER_TX 34


void ThreeWayValveBoard::Init(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");

    // __barcode_reader.Init(PIN_BARCODE_READER_RX, PIN_BARCODE_READER_TX);
    __go_staight_stopper.Init('T', PIN_GO_STRAIGHT_STOPPER, HIGH);
    __turn_right_stopper.Init('L', PIN_TURN_RIGHT_STOPPER, HIGH);

    __h_bridge.Init(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_PWM_SPEED);
    __dc_motor.LinkHBridgeDriver(&__h_bridge);

    __reciprocator.LinkActuator(&__dc_motor);
    __reciprocator.LinkPositionTriger(&__go_staight_stopper, &__turn_right_stopper);
    __valve.LinkReciprocator(&__reciprocator);

    __inlet_ir_sensor.Init('I', PIN_INLET_IR_SENSOR, HIGH);
    __outlet_ir_sensor_go_straight.Init('T', PIN_OUTLET__IR_SENSOR_GO_STRAIGHT, HIGH);
    __outlet_ir_sensor_turn_right.Init('L', PIN_OUTLET_IR_SENSOR_TURN_RIGHT, HIGH);
    __valve.LinkIrSensors(&__inlet_ir_sensor, &__outlet_ir_sensor_go_straight, &__outlet_ir_sensor_turn_right);

    __led_go_straight_blue.Init('B', PIN_LED_GO_STRAIGHT_BLUE, HIGH);
    __led_go_straight_red.Init('R', PIN_LED_GO_STRAIGHT_RED, HIGH);
    __led_go_straight_green.Init('G', PIN_LED_GO_STRAIGHT_GREEN, HIGH);
    __led_turn_right_blue.Init('b', PIN_LED_TURN_RIGHT_BLUE, HIGH);
    __led_turn_right_red.Init('r', PIN_LED_TURN_RIGHT_RED, HIGH);
    __led_turn_right_green.Init('g', PIN_LED_TURN_RIGHT_GREEN, HIGH);

    __valve.LinkLeds(&__led_go_straight_green, 
                    &__led_go_straight_red,
                    &__led_go_straight_blue,
                    &__led_turn_right_green,
                    &__led_turn_right_red,
                    &__led_turn_right_blue );
}

