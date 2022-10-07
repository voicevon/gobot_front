#include "vsc_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
// #include "Robot/mcode_runner/mcode_runners.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"

void VscRobot::Init(Vsc_Board* board){
    Logger::Debug("VscRobot::Init()");
    this->_cnc_board = board;
    this->_LinkEef(board->GetEef());

    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    this->__planner.__arm_solution = &arm_solution;
    this->_arm_solution = &this->arm_solution;  

    this->__g28_runner = &this->g28_runner;
    g28_runner.Init(&mover);

    this->LinkMover(&mover);
    this->__Init_actuators(board);
    this->__Init_pids();

}

void VscRobot::__Init_actuators(Vsc_Board* board){
    Logger::Info("VscRobot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);

    this->__actuator_alpha.LinkPidController(&__speed_pid);
    this->__actuator_alpha.LinkHBridgeDriver(board->GetHBridgeDriver());
    this->__actuator_alpha.LinkEncoder(board->GetEncoder());
    this->__actuator_alpha.MyName = 'A';
    
    ActuatorEncoder_Calculator helper;
    helper._motor_gear_pitch_in_mm = 12.7;
    helper._slave_pulley_teeth_count = 368;
    float slope = helper.GetActuatorToCncFormular_Slope_raw_per_rad();
    __actuator_alpha.Init_FomularSlope(slope);


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




    // // this->_micro_steps_on_stepper_driver = 16;
    // // this->_motor_gear_teeth_count = 10;
    // // this->_slave_pulley_teeth_count = 90;
    // // this->_motor_step_angle_in_degree = 0.7003891050583658;

    //     Logger::Info("Vsc_ArmSoution::Init() angle sensor and motor");
//     board->GetAngleSensor()->SetupFormular(0.027173913f, 0.0f);   // slope = (10 / 56) * (56/368)
//     board->EnableMotor(AXIS_ALPHA, false);

//     Logger::Info("Vsc_ArmSoution::Init() Kinematic_config");
//     CircleLoop_KinematicConfig* kinematic = &this->_kinematic_config;
//     kinematic->CircleLength = 12.7 * 368;
//     kinematic->PitchLength = 12.7;  
}