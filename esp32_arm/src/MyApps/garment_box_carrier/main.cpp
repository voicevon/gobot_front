#include "all_devices.h"
#ifdef I_AM_GARMENT_BOX_CARRIER

// #include "box_carrier_board.h"

#include "MyLibs/MyFunctions.hpp"
#include "box_carrier.h"
#include "box_carrier_hw.h"
#include "IoT/main_mqtt.h"
#include "Myapps/garment_bot_single_mcu/board_ver2.0.h"


BoxCarrier *robot;
BoxCarrierHardware* robot_hw;
GcodeQueue* gcode_queue;
MessageQueue* mqtt_command_queue;


void setup_robot_hardware(){
    BoardSingleMcu_ver2_0* board = new BoardSingleMcu_ver2_0();
    board->Init();
    robot_hw = new BoxCarrierHardware(board);

    Adafruit_MCP23X17* board_mcp = board->GetMcp23018();
    SingleAxisHomer* z_homer = new SingleAxisHomer(board_mcp, MC23018_PIN_HOME_Z, LOW);
    SingleAxisHomer* y_homer = new SingleAxisHomer(board_mcp, MC23018_PIN_HOME_Y, LOW);
    robot_hw->LinkHomer(z_homer, y_homer);

    Stepper* alpha_stepper = new Stepper(PIN_ALPHA_STEP, board_mcp, MC23018_PIN_ALPHA_DIR);
    Stepper* beta_stepper = new Stepper(PIN_BETA_STEP, board_mcp, MC23018_PIN_BETA_DIR);
    robot_hw->LinkStepper(alpha_stepper, beta_stepper);
    robot_hw->InitRobot();
}


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garment box-Carrier for Jetty .  Keep smiling :)");
    setup_robot_hardware();

    robot = new BoxCarrier();
    gcode_queue = new GcodeQueue();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hw->LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    setup_mqtt_block_connect();
    mqtt_command_queue = new MessageQueue();
    append_mqtt_bridge("puma/x2212/bm", mqtt_command_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");

    robot->ExecuteMqttCommand("G28Y");
}

void loop(){
    robot->SpinOnce();
    robot_hw->SpinOnce();
    loop_mqtt();
}

#endif