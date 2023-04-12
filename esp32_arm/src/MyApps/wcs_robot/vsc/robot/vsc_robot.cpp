#include "vsc_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"

void VscRobot::Init(Vsc_Board* board){
    Logger::Debug("VscRobot::Init()");
    // this->_cnc_board = board;

    // gs_MoveBlock_Queue::Instance()._all_elements = this->__all_move_blocks;
    // this->__planner.__arm_solution = &arm_solution;
    // this->_arm_solution = &this->arm_solution;  

    this->_g28_runner = &this->g28_runner;
    g28_runner.Init(&mover, &arm_solution,"");

    this->_LinkMover(&mover);
    this->_InitStatic_Queues();
    this->_Init_ArmSolution();
    this->__Init_actuators(board);
    this->__Init_pids();
    this->__Init_layer_position();
}

void VscRobot::__Init_layer_position(){
    for (int i=0; i<STORE_LAYER_COUNT; i++){
        __position_in_pitch[i] = 0;
    }
    //  0 ~ 156
    this->__position_in_pitch[0] = 0;
    this->__position_in_pitch[1] = 13;
    this->__position_in_pitch[2] = 26;
    this->__position_in_pitch[3] = 39;
    this->__position_in_pitch[4] = 52;
    this->__position_in_pitch[5] = 65;
    this->__position_in_pitch[6] = 78;
    this->__position_in_pitch[7] = 91;
    this->__position_in_pitch[8] = 104;
    this->__position_in_pitch[9] = 117;
    this->__position_in_pitch[10] = 130;
    this->__position_in_pitch[11] = 143;
    this->__position_in_pitch[12] = 156;
    this->__position_in_pitch[13] = 169;
    this->__position_in_pitch[14] = 182;

    Serial.print("\n[Info] VscApp::VscApp() is constructed");
}

void VscRobot::MoveTo(int layer_index, int cell_index){
    String g1 = "G1A";
    // TODO:  over single circle.
    float gear_angle = TWO_PI *  this->__position_in_pitch[layer_index] / 184;
    g1.concat(gear_angle);
    g1.concat("F0.1");
    this->_gcode_queue.AppendGcodeCommand(g1);
}

void VscRobot::__Init_actuators(Vsc_Board* board){
    Logger::Info("VscRobot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);

    this->__actuator_alpha.LinkPidController(&__speed_pid);
    this->__actuator_alpha.LinkHBridgeDriver(board->GetHBridgeDriver());
    this->__actuator_alpha.LinkEncoder(board->GetEncoder());
    this->__actuator_alpha.MyName = 'A';
    
    // ActuatorEncoder_Calculator helper;
    // helper.motor_gear_pitch_in_mm = 12.7;
    // helper.slave_pulley_teeth_count = 368;
    // float slope = helper.Get_Formular_Slope_raw_per_rad();
    // __actuator_alpha.Formular_SetSlope(slope);


}

void VscRobot::_InitStatic_Queues() {

}

void VscRobot::_Init_ArmSolution() {

}
void VscRobot::__Init_pids(){

    Logger::Info("VscRobot::Init() Pid controllers.");
    PidControllers_List::Instance().Init(__all_pids, PID_CONTROLLERS_COUNT);
    PidControllers_List::Instance().AddPidController(&this->__speed_pid);
    // PidControllers_List::Instance().AddPidController(&this->__test_pid);

    // Logger::Print("test",PidControllers_List::Instance().GetPidController(1)->P);
    // Logger::Print("test",PidControllers_List::Instance().GetPidController(1)->I);
    // Logger::Print("test",PidControllers_List::Instance().GetPidController(1)->D);
    // Logger::Halt("");
    
    // motor is the user of PID controller,  initial setting
    Logger::Info("VscRobot::Init()  Speed Pid controller.");
    __speed_pid.P = 100.0f;
    __speed_pid.I = 100.0f;
    __speed_pid.D = 0.0f;

}