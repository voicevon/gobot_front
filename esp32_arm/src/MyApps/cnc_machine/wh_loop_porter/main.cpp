
#include "board/board.h"
#include "loop_porter_app.h"
#include "robot/loop_porter_robot.h"
#include "Mqtt/from_mqtt_client_to_remote_queue.h"

#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_LOOP_PORTER

#define MY_ROBOT_ROW_ID 1
#define MQTT_TOPIC_GCODE "twh/221109/r1/gcode"   
#define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408twh/221109/r1/state"

GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

Twh_LoopPorter_Board board;
Twh_LoopPorter_App app(MY_ROBOT_ROW_ID);
Twh_LoopPorter_Robot robot;




int latchPin=32; //ST CP of 74HC595//load
int clockPin=33;//SH CP of 74HC595//sclk
int dataPin=12; //DS of 74HC595//sdi
unsigned char smgduan[] = {  //共阳
  B11000000, // 0
  B11111001, // 1
  B10100100, // 2
  B10110000, // 3
  B10011001, // 3
  B10010010, // 8
  B10000010, // 6
  B11111000, // 7
  B10000000, // 8
  B10010000, // 9
  };//显示0~9的值共阴
//unsigned char duanMa[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; //选择1-8哪个数码管段
void setu22p() {
      pinMode(latchPin, OUTPUT);
      pinMode(clockPin, OUTPUT);
      pinMode(dataPin, OUTPUT); 
}
void loo22p()
{
   for(int i=0; i<9999; i++) {
    unsigned char gewei = (i%100)%10;
    unsigned char shiwei = (i%100)/10;
    // unsigned char baiwei = (i%1000)/100; 
    // unsigned char qianwei = i/1000; 
    for (int x=0; x<8; x++){

      digitalWrite(latchPin,LOW); //低电位表示启动
      shiftOut(dataPin,clockPin,MSBFIRST,255- (1<<x));
      shiftOut(dataPin,clockPin,MSBFIRST,255- (1<<x));
    //   shiftOut(dataPin,clockPin,MSBFIRST,smgduan[gewei]);
    //   shiftOut(dataPin,clockPin,MSBFIRST,smgduan[shiwei]);
    //   shiftOut(dataPin,clockPin,MSBFIRST,smgduan[baiwei]);
    //   shiftOut(dataPin,clockPin,MSBFIRST,smgduan[qianwei]);
      digitalWrite(latchPin,HIGH); //高电位表示停止
    //   delayMicroseconds(2);
    //   digitalWrite(latchPin, HIGH);//ST_CP
      delay(1000);
    }
   }

}


void test_board(){
    board.GetDisplayer()->Test(9999, 2);
    // board.Test_PositionTriggers(0);
    // board.Test_Stepper(0);
    // Serial.println("[Info] test_board() is done  MY_ROBOT_ROW_ID=" + String(MY_ROBOT_ROW_ID));
}

void test_robot(){
    for (int i=0; i<6;i++){

        // gcode_queue.AppendGcodeCommand("G1X-90Y0");
        // gcode_queue.AppendGcodeCommand("G4S0");

        // gcode_queue.AppendGcodeCommand("G1X-90Y200");
        // gcode_queue.AppendGcodeCommand("G4S3");



        gcode_queue.AppendGcodeCommand("G1X-190Y0");
        gcode_queue.AppendGcodeCommand("G4S5");

        gcode_queue.AppendGcodeCommand("G1X0Y0");
        gcode_queue.AppendGcodeCommand("G4S5");

        gcode_queue.AppendGcodeCommand("G1X190Y0");
        gcode_queue.AppendGcodeCommand("G4S5");
    }
    
}

void test_arm(){
    // Twh2_Circleloop_ArmSolution arm;
    // FKPosition_XYZRPW input_fk;
    // input_fk.X = 123;
    // input_fk.Y = 234;
    // input_fk.Z = 12;
    // arm.Test_FK_MK_FK(&input_fk);

    // IKPosition_abgdekl input_ik;
    // input_ik.Positions[AXIS_ALPHA] = 456;
    // input_ik.Positions[AXIS_BETA] = -567;
    // arm.Test_IK_MK_IK(&input_ik);

    // MiddleKinematic input_mk;
    // input_mk.Arm_shaft_At_Y = -12.3f;
    // input_mk.Arm_Angle = -23.4f * DEG_TO_RAD;
    // arm.Test_MK_IK_MK(&input_mk);
    // arm.Test_MK_FK_MK(&input_mk);

    while (true){

    }
}

void setup(){
    board.Init();
    // test_arm();
    Logger::Debug("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    board.GetDisplayer()->TestHardware();
    // test_board();

    // float xx = Twh2_Circleloop_Armsolution_Config().Slope_Steps_per_box();
    
    robot.Init(&board);

    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    mono_remote_queue_bridge_via_mqtt_setup(MQTT_TOPIC_GCODE, &mqtt_command_queue, &app); 

    gcode_queue.AppendGcodeCommand("G28X");
    gcode_queue.AppendGcodeCommand(MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP);
    // test_robot();
    Logger::Info ("App::loop_porter    setup() is done. ");
    
}


void loop(){

    // Logger::Print("Arduino loop() point ", 1);
    app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    robot.SpinOnce();
    robot.MySpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    mono_remote_queue_bridge_spin_once();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif
