#include "board_cable_bot_corner.h"

#define PIN_HOME_SENSOR 5
#define PIN_ENCODER_A 6
#define PIN_ENCODER_B 7
#define PIN_DC_MOTOR_A 8
#define PIN_DC_MOTOR_B 9


#ifdef I_AM_CABLE_AXIS_XPYP
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "1fd03a14-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "4b7fbe96-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "50711238-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XNYP
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "6909288a-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "6e0d5d7e-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "72838810-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XNYN
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "77d37e60-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "8fdb27b0-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "8175103c-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XPYN
  #define BLE_DEV_NAME "ConInt X+Y+"
  #define SERVICE_UUID      "85f5ab94-1451-11ec-82a8-0242ac130003"
  #define ROBOT_STATE_UUID  "96a1cd38-1451-11ec-82a8-0242ac130003"
  #define ROBOT_ACTION_UUID "9c43b29c-1451-11ec-82a8-0242ac130003"
#endif


Board_CableBotCorner::Board_CableBotCorner(){
    driver = new DCDriverHBridge(PIN_DC_MOTOR_A, PIN_DC_MOTOR_B);
    encoder = new Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 200);
}

void Board_CableBotCorner::Init(){
}
