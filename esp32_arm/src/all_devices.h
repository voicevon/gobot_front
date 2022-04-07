#ifndef __ALL_DEVICES_H_
#define __ALL_DEVICES_H_

// #define I_AM_AGV_WHEEL
// #define I_AM_GARMENT_BOT
// #define I_AM_GARMENT_BOX_MOVER
// #define I_AM_GOBOT_HOUSE_2134
#define I_AM_GOBOT_CHESSBOARD_2134

// #define I_AM_ACUPUCTURE_MAIN_ESP32
// #define I_AM_SMOKE_MACHINE_WEB_SERVER
// #define I_AM_CABLE_AXIS_XPYP
// #define I_AM_CABLE_AXIS_XNYP
// #define I_AM_CABLE_AXIS_XNYN
// #define I_AM_CABLE_AXIS_XPYN


	// #define I_AM_SMOKE_MACHINE_MQTT

#ifdef I_AM_GOBOT_CHESSBOARD_2134
	#define I_AM_GOBOT_CHESSBOARD
	#define USING_WIFI_MQTT
	#define MQTT_SYNCERS_COUNT  1

	// #define USING_BLE
	#ifdef USING_BLE
		#define BLE_DEV_NAME "Gobot-Arm-213401"
		#define BLE_SERVICE_UUID "d592c9aa-0594-11ec-9a03-0242ac130003"
		#define BLE_COMMU_UUID "b7c65186-0610-11ec-9a03-0242ac130003"
		#define BLE_STATE_UUID "c21a1596-0610-11ec-9a03-0242ac130003"
	#endif
#endif

#ifdef I_AM_GOBOT_HOUSE_2134
	#define I_AM_GOBOT_HOUSE
	#define USING_WIFI_MQTT
	#define MQTT_SYNCERS_COUNT  1   

	// #define USING_BLE
	#ifdef USING_BLE
		#define BLE_DEV_NAME "Gobot-House-213401"
		#define BLE_SERVICE_UUID  "b416890c-062e-11ec-9a03-0242ac130003"
		#define BLE_COMMU_UUID "bfa35098-062e-11ec-9a03-0242ac130003"
		#define BLE_STATE_UUID  "c52ca230-062e-11ec-9a03-0242ac130003"
	#endif
#endif

#ifdef I_AM_GARMENT_BOT
	#define USING_WIFI_MQTT
	#define MQTT_SYNCERS_COUNT  1   
#endif

#ifdef I_AM_GARMENT_BOX_MOVER
	#define USING_WIFI_MQTT
	#define MQTT_SYNCERS_COUNT  1   
#endif

#ifdef I_AM_ACUPUCTURE_MAIN_ESP32
	#define USING_WIFI_MQTT
	#define MQTT_SYNCERS_COUNT  0   
    #define BODY_ID "001"
#endif

#ifdef I_AM_CABLE_AXIS_XPYP
	#define I_AM_CABLEBOT_CORNER
	#define USING_BLE
	#define BLE_DEV_NAME "ConInt-XPYP"
	#define AXIS_NAME  'A'
	#define BLE_SERVICE_UUID  "1fd03a14-1451-11ec-82a8-0242ac130003"
	#define BLE_COMMU_UUID "4b7fbe96-1451-11ec-82a8-0242ac130003"
	#define BLE_STATE_UUID  "50711238-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XNYP
	#define I_AM_CABLEBOT_CORNER
	#define USING_BLE
	#define AXIS_NAME 'B'
	#define BLE_DEV_NAME "ConInt-XNYP"
	#define BLE_SERVICE_UUID  "6909288a-1451-11ec-82a8-0242ac130003"
	#define BLE_COMMU_UUID "6e0d5d7e-1451-11ec-82a8-0242ac130003"
	#define BLE_STATE_UUID "72838810-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XNYN
	#define I_AM_CABLEBOT_CORNER
	#define USING_BLE
	#define AXIS_NAME 'C'
	#define BLE_DEV_NAME "ConInt-XNYN"
	#define BLE_SERVICE_UUID  "77d37e60-1451-11ec-82a8-0242ac130003"
	#define BLE_COMMU_UUID "8fdb27b0-1451-11ec-82a8-0242ac130003"
	#define BLE_STATE_UUID  "8175103c-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_CABLE_AXIS_XPYN
	#define I_AM_CABLEBOT_CORNER
	#define USING_BLE
	#define AXIS_NAME 'F'
	#define BLE_DEV_NAME "ConInt-XPYN"
	#define BLE_SERVICE_UUID   "85f5ab94-1451-11ec-82a8-0242ac130003"
	#define BLE_COMMU_UUID  "96a1cd38-1451-11ec-82a8-0242ac130003"
	#define BLE_STATE_UUID   "9c43b29c-1451-11ec-82a8-0242ac130003"
#endif

#ifdef I_AM_SMOKE_MACHINE_WEB_SERVER
	#define USING_WIFI_MQTT
	#define AXIS_NAME 'X'
#endif





#endif