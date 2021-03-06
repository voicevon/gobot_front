#include "all_applications.h"
#ifdef I_AM_ROBOT_ASRS_AGV


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "IoT/main_mqtt.h"
// #include "MyApps/box_carrier/cnc_machine.h"
// #include "../box_carrier/board/mechanic/cnc_machine.h"
// #include "MyApps/box_carrier/board/mechanic/cnc_solution_config.h"
// #include "MyApps/box_carrier/board/mechanic/cnc_solution.h"
#include "MyApps/box_carrier/cnc/solution.h"
#include "MyApps/box_carrier/cnc/solution_config.h"

StepControl objStepControl;  // This object can not inside any object?
BoardAllInOne board = BoardAllInOne();
// BoxCarrier_CncSolutionConfig cncMachine;
BotAsrsAgvCoreYZ robot= BotAsrsAgvCoreYZ(ROBOT_SERIAL_ID);
MessageQueue garment_bot_message_queue = MessageQueue();


#include "MyBoards/board_test/board_test_agv.h"
#include "MyBoards/board_test/board_test_asrs.h"
#include "MyBoards/board_test/board_test_cnc.h"


void unit_test(){
    UnitTestAgv agv;
    agv.LinkBoard(&board.agv);
    agv.Test_TrackLight(0);
    agv.Test_Battery(0);
    agv.Test_ObstacleSensor(0);
    agv.Test_TrackSensor(0);
    agv.Test_RfidReader(0);
    agv.Test_Mover(0);

    BoardTestCnc cnc;
    cnc.LinkBoard(&board.cnc_board);
    cnc.Test_AllHomers(0);
    // cnc.Test_Stepper(0,'A', 5000, &objStepControl);
    // cnc.Test_Stepper(0,'B', 5000, &objStepControl);

    UnitTestAsrs asrs;
    asrs.LinkBoard(&board.asrs);
    // asrs->Test_Sharp_IrSensor(20);
}

void function_test(){
    // robot.Test_HomeZ();
    // robot.Test_HomeY();
    // robot.Test_UnHome();

}

void setup(){
    board.Init(true);
    unit_test();
    board.Test_ScanI2cBuses(0);

    // cncMachine.Init('S');
    robot.InitAllinOne(&board, &objStepControl);

   // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    String mqtt_topic = "puma/bot/xROBOT_SERIAL_ID";
    mqtt_topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), &garment_bot_message_queue, &robot); 
    setup_mqtt_on_message_receive(); 

    board.RepportRamUsage();
    Serial.println ("\n[Info] garment agv  setup() is done. ------------------------------------  ");
    function_test();
}

int64_t last_report_timestamp = 0;

void mqtt_report_states(){
    String topic = "puma/bot/states";
    uint8_t payload[60]; 
    robot.GetMqtt_PubPayload(&payload[0]);
    const char * pp = (const char*)(&payload[0]);
    mqttClient.publish(topic.c_str(), 2, true, pp);
}

void loop(){
    robot.SpinOnce();
    loop_mqtt();
    int64_t time_since_boot = esp_timer_get_time();
    if (time_since_boot - last_report_timestamp  > 30000000){  // 30 second
        mqtt_report_states();
        last_report_timestamp = time_since_boot;        
    }
}

#endif