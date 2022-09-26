// #include "gcode_dispatch.h"
// #include "MyLibs/basic/logger.h"

// void GcodeDispatch::RunGcode(Gcode* gcode){
//     std::string result;
//     // if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND)){
//     //   Serial.print("RunGcode()   OK or Unknown");
//     //   return;
//     // }
//     bool debug = false;
//     Logger::Debug("CncSolutionBase::RunGcode()");
//     Logger::Print("gcode_command", gcode->get_command());

//     if(gcode->has_g){
//         char home_axis = '+';
//         switch (gcode->g){
//         case 28:
//             // G28: Home
//             this->State = CncState::RUNNING_G28;
//             if (gcode->has_letter('X')) home_axis='X';
//             if (gcode->has_letter('Y')) home_axis='Y';
//             if (gcode->has_letter('Z')) home_axis='Z';
//             if (gcode->has_letter('A')) home_axis='A';
//             if (gcode->has_letter('B')) home_axis='B';
//             if (gcode->has_letter('C')) home_axis='C';
//             if (gcode->has_letter('W')) home_axis='W';

//             // Is there any machine that supports both IK, and FK homing?
//             // this->_home_via_inverse_kinematic = false;
//             if (home_axis == '+'){
//                 Serial.print("\n\n\n\n[Error] CncSolutionBase::RunGcode()  :");
//                 Serial.print(home_axis);

//             }
//             //TODO:  convert char to enum
//             this->RunG28(this->ConvertToEnum(home_axis));
//             // this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'
//             break;

//         case 1:
//             // G1 Move
//             //TODO:  1. put position to movement queue. called "plan" in smoothieware? 
//             //       2. send out OK.
//             //       3. Set status to busy.
//             //       4. Start Moving.
//             this->State = CncState::RUNNING_G1;
//             this->RunG1(gcode);
//             // this->commuDevice->OutputMessage(COMMU_OK);
//             break;
//         case 4:
//             // G4 Dwell, Pause for a period of time.
//             this->State = CncState::RUNNING_G4;
//             this->RunG4(gcode);
//             break;
//         case 6:
//             this->RunG6(gcode);
//             // this->commuDevice->OutputMessage(COMMU_OK);
//             break;
//         case 90:
//             // Absolute position
//             this->is_absolute_position = true;
//             // this->commuDevice->OutputMessage(COMMU_OK);
//             break;
//         case 91:
//             // Relative position
//             this->is_absolute_position = false;
//             // this->commuDevice->OutputMessage(COMMU_OK);
//             break;
//         // case 92:
//             // Set Position     G92 X10 E90
//             // break;
//         default:
//             break;
//         }
//     }else if(gcode->has_m){
//         uint8_t p_value = 33;   //TODO: Make sure this is no harmful!
//         uint8_t s_value = 0;
//         float f_value = 0.0f;
//         EefAction action;
//         switch (gcode->m){
//         case 42:
//             p_value =  gcode->get_value('P');
//             s_value = gcode->get_value('S');
//             this->Run_M42_OutputGpio(p_value, s_value);
//         case 84:
//             this->RunM84();
//         case 114:
//             // Get Current Position
//             break;

//         case 119:
//             // Get Endstop Status
//             result = GetHomeTrigerStateString();
//             // this->commuDevice->OutputMessage(COMMU_OK);
//             Serial.print(result.c_str());
//             // this->commuDevice->WriteNotification(result);
//             break;

//         case 123:
//             //M123 P=channel_index, S=Set EEF action			
//             while (this->State != CncState::IDLE){
//                 this->SpinOnce();
//             }
//             p_value =  gcode->get_value('P');
//             s_value = gcode->get_value('S');
//             debug = true;
//             if (debug){
//                 Logger::Debug("CncSolutionBase::RunGcode() For EEF_ACTION  M123 ");
//                 Logger::Print("P", p_value);
//                 Logger::Print("S", s_value);
//             }
//             action = (EefAction)s_value;
//             this->RunM123(p_value, s_value);
//             break;

//         case 130:
//             Logger::Debug("CncSolutionBase::RunGcode()   M130");
//             Logger::Print("gcode", gcode->get_command());
//             p_value =  gcode->get_value('N');
//             Logger::Print("Index", p_value);
            
//             f_value = gcode->get_value('P');
//             this->__pid_controllers->GetController(p_value)->P = f_value;
//             Logger::Print("P", f_value);

//             f_value = gcode->get_value('I');
//             this->__pid_controllers->GetController(p_value)->I = f_value;
//             Logger::Print("I", f_value);

//             f_value = gcode->get_value('D');
//             this->__pid_controllers->GetController(p_value)->D = f_value;
//             Logger::Print("D", f_value);

//             break;

//         case 141:
//             break;

//         case 280:
//             // Set servo position  
//             //	 	Pnnn Servo index
//             // 		Snnn Angle or microseconds
//             // Wait for all gcode, mcode is finished
//             // Serial.println("M280 Started");
//             while (this->State != CncState::IDLE){
//                 this->SpinOnce();
//             }
//             if (gcode->has_letter('P')) p_value = gcode->get_value('P');
//             if (gcode->has_letter('S')) s_value = gcode->get_value('S');
//             ledcWrite(p_value, s_value);   // from ledcWrite(ledChannel, dutyCycle);
//             // this->commuDevice->OutputMessage(COMMU_OK);
//             // this->commuDevice->WriteNotification("IDLE");
//             break;
//         case 996:
//             // Do nothing. this should be the last gcode of a movement in transaction.
//             // after MCU reset, This should be the first gcode it received, even the message queue is nothing.
//             break;
//         default:
//             break;
//         }
//     }else{
//         // this->commuDevice->OutputMessage("\n[Warning] CncSolutionBase::RunGcode()  Has NO letter 'G' or 'M'. ");
//         // this->commuDevice->OutputMessage(gcode->get_command());
//         // this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
//     }
// }