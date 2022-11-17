#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE

#include "MyApps/teeth_wh/board/teeth_wh_board.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "MyApps/teeth_wh/teeth_wh_app.h"
#include "MyApps/teeth_wh/robot/teeth_wh_robot.h"


TeethWarehouse_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

TeethWarehouse_App app;
TeechWarehouse_Robot robot;
// #include "FastAccelStepper.h"
// #include "MyLibs/basic/logger.h"


// FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
// FastAccelStepper* __stepper_alpha = NULL;
// FastAccelStepper* __stepper_beta = NULL;

void test_board(){
    board.Test_PositionTriggers(0);
    board.Test_Servo_AirPen(0);
    board.Test_Servo_AirSwitch(0);
    board.Test_VacuumPump(0);
    board.Test_SingleStepper(1, 0);
    board.Test_DualStepper(0);
    Serial.println("[Info] test_board() is done.");
}

bool running_flag = false;
void setup(){
   
    board.Init(true);
    test_board();
    robot.Init(&board);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    setup_mqtt_block_connect();
    append_mqtt_bridge("teeth_wh/wh2210", &mqtt_command_queue, &app); 
    setup_mqtt_on_message_receive(); 
    gcode_queue.AppendGcodeCommand("G28A");
    gcode_queue.AppendGcodeCommand("G28Y");


    // Serial.begin(115200);
    // Serial.println("I am Teeth Warehouse.");
    // __stepper_engine.init(1);
    // __stepper_alpha = __stepper_engine.stepperConnectToPin(26);  // for alpha=27, for beta=12
    // __stepper_beta = __stepper_engine.stepperConnectToPin(12);  // for alpha=27, for beta=12

    //     if (__stepper_alpha) {
    //     __stepper_alpha->setDirectionPin(27);   //for alpha=26, for beta = 14
    //     __stepper_alpha->setEnablePin(13);                            
    //     __stepper_alpha->setAutoEnable(true);

    //     __stepper_alpha->setSpeedInUs(1000);  // the parameter is us/step !!!
    //     __stepper_alpha->setAcceleration(100);
    //     int res =  __stepper_alpha->moveTo(-1000, false);
    //     Logger::Print("moveTo() returns", res);
    //     Logger::Info("TeethWarehouse_Board::Init()");
    //     Logger::Print("stepper alpha is OK.", 0);
    //     running_flag = true;
    // }else{
    //     Logger::Error("TeethWarehouse_Board::Init() ");
    //     Logger::Halt("failed FastAccelStepper.");
    // }


    // if (__stepper_beta) {
    //     __stepper_beta->setDirectionPin(14);   //for alpha=26, for beta = 14
    //     __stepper_beta->setEnablePin(13);                            
    //     __stepper_beta->setAutoEnable(true);

    //     __stepper_beta->setSpeedInUs(1000);  // the parameter is us/step !!!
    //     __stepper_beta->setAcceleration(100);
    //     int res =  __stepper_beta->moveTo(1000, false);
    //     Logger::Print("moveTo() returns", res);
    //     Logger::Info("TeethWarehouse_Board::Init()");
    //     Logger::Print("stepper alpha is OK.", 0);
    //     running_flag = true;
    // }else{
    //     Logger::Error("TeethWarehouse_Board::Init() ");
    //     Logger::Halt("failed FastAccelStepper.");
    // }


    Logger::Info ("Teeth Warehouse   setup() is done. ");
}

void loop(){
    // board.Test_Stepper(0, 9);
    // if (__stepper_alpha->isRunning()){
    //     Serial.print("__stepper_alpha position=  ");
    //     Serial.print(__stepper_alpha->getCurrentPosition());
    //     Logger::Print("     Current position", __stepper_beta->getCurrentPosition());
    // }else{
    //     if (running_flag){
    //         running_flag = false;
    //         Logger::Print("Stepper is not running.",0);
    //     }
    // }
    // delay(300);

    // return;

    // Logger::Print("Arduino loop() point ", 1);
    app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    robot.SpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    loop_mqtt();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif
