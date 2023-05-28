#include "board.h"



#define PIN_LED_GO_STRAIGHT_BLUE 17
#define PIN_LED_GO_STRAIGHT_RED 18
#define PIN_LED_GO_STRAIGHT_GREEN 19
#define PIN_LED_TURN_RIGHT_BLUE 20
#define PIN_LED_TURN_RIGHT_RED 21
#define PIN_LED_TURN_RIGHT_GREEN 22
#define PIN_H_BRIDGE_PWM_SPEED 23
#define PIN_H_BRIDGE_DIR 24
#define PIN_GO_STRAIGHT_STOPPER 15
#define PIN_TURN_RIGHT_STOPPER 16


void ThreeWayValveBoard::Init(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");

    __go_staight_stopper.Init('T', PIN_GO_STRAIGHT_STOPPER, HIGH);
    __turn_right_stopper.Init('L', PIN_TURN_RIGHT_STOPPER, HIGH);

    __led_go_straight_blue.Init('B', PIN_LED_GO_STRAIGHT_BLUE, HIGH);
    __led_go_straight_red.Init('R', PIN_LED_GO_STRAIGHT_RED, HIGH);
    __led_go_straight_green.Init('G', PIN_LED_GO_STRAIGHT_GREEN, HIGH);
    __led_turn_right_blue.Init('b', PIN_LED_TURN_RIGHT_BLUE, HIGH);
    __led_turn_right_red.Init('r', PIN_LED_TURN_RIGHT_RED, HIGH);
    __led_turn_right_green.Init('g', PIN_LED_TURN_RIGHT_GREEN, HIGH);


    __h_bridge.Init(PIN_H_BRIDGE_DIR, PIN_H_BRIDGE_PWM_SPEED);
    __dc_motor.LinkHBridgeDriver(&__h_bridge);

    __reciprocator.LinkActuator(&__dc_motor);
    __reciprocator.LinkPositionTriger(&__go_staight_stopper, &__turn_right_stopper);
    __valve.LinkReciprocator(&__reciprocator);
}

