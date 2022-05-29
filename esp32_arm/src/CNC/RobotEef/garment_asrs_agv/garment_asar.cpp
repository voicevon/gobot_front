#include "garment_asar.h"

RobotEef_GarmentAsar::RobotEef_GarmentAsar(){
	
}

void RobotEef_GarmentAsar::Run(uint8_t eef_code) {
    	// Serial.println(eef_action);

    //         Lower = 1,
    // Higher = 2, 
    // // also Load
    // Suck = 3,  
    // Load = 3,
    // // also Unload
    // Release = 4,  
    // Unload = 4,
    // Sleep = 5,


	// switch (eef_action){
	// 	case EefAction::Lower:
	// 		this->eefServo->write(180);
	// 		break;
	// 	case EefAction::Higher:
	// 		this->eefServo->write(0);
	// 		break;
	// 	case EefAction::Suck:
	// 		digitalWrite(PIN_EEF_A, HIGH);
	// 		digitalWrite(PIN_EEF_B, LOW);
	// 		break;
	// 	case EefAction::Release:
	// 		digitalWrite(PIN_EEF_A,LOW);
	// 		digitalWrite(PIN_EEF_B,HIGH);
	// 		break;
	// 	case EefAction::Sleep:
	// 		digitalWrite(PIN_EEF_A,LOW);
	// 		digitalWrite(PIN_EEF_B,LOW);
	// 		break;
	// 	default:
	// 		break;

	// }
}