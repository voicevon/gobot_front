
#include "board/board.h"
#include "von/utility/mqtt/subscriber/mqtt_subscriber_manager.h"
#include "MyLibs/mqtt/remote_component/remote_var_chars.h"
// #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


#include "../select_app.h"
#ifdef I_AM_DENTURE_STATION_MOUNT_TEETH

#define MY_WORKSTATION_ID       "101"
#define WARHOUSE_ID             "221109"



ThreeWayValveBoard board;
// RemoteVar_Chars remote_commander;

// void setup_wifimanager(){

//     // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
//     // it is a good practice to make sure your code sets wifi mode how you want it.

//     // put your setup code here, to run once:
//     Serial.begin(115200);
//     Logger::Debug("Setting up WifiManager,,,,");
    
//     //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
//     WiFiManager wm;

//     // reset settings - wipe stored credentials for testing
//     // these are stored by the esp library
//     // wm.resetSettings();

//     // Automatically connect using saved credentials,
//     // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
//     // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
//     // then goes into a blocking loop awaiting configuration and will return success result

//     bool res;
//     // res = wm.autoConnect(); // auto generated AP name from chipid
//     // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
//     res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

//     if(!res) {
//         Serial.println("Failed to connect");
//         // ESP.restart();
//     } 
//     else {
//         //if you get here you have connected to the WiFi    
//         Serial.println("connected...yeey :)");
//     }
// }

void setup(){
    #define MQTT_TOPIC_OF_ORDER_REQUEST "wh" WARHOUSE_ID "/station"  MY_WORKSTATION_ID  "/order"
    // setup_wifimanager();

    board.Init();

    // setup_wifi_mqtt_blocking_mode();

    // const char* MQTT_TOPIC_FOR_TUBER_INLET = "wh221109/tube1/vvinlet1/command";
    // gs_MqttSubscriberManager::Instance().AddSubscriber(MQTT_TOPIC_FOR_TUBER_INLET, &remote_commander);
    // Logger::Info ("App::loop-porter::setup() is done. ");
    board.TestPositionTriggers(9999);

}


// Command from:
// 1. Remote_commander
// 2. Gpio button
void FollowCommander(){
    // char* command = remote_commander.Get();
    // // if (command == "go_straight"){
    // if (true){
    //     board.GetValve()->SetMode_GoStraight();
    // }
    // // if (command == "turn_right"){
    // else if (true){
    //     board.GetValve()->SetMode_TurnRight();
    // }
    // // Read GPIO button
    // else if (true){
    //     board.GetValve()->SetMode_GoStraight();
    // }
    // else if (true){
    //     board.GetValve()->SetMode_TurnRight();
    // }
}

void loop(){
    // if (board.GetValve()->GetState() == ThreeWayValve::EnumState::STATE_IDLE){
    //     FollowCommander();
    // }
    // board.GetValve()->SpinOnce_Statemachine();

    // if (board.GetBarcodeReader()->Read()){
    //     char* order_id = board.GetBarcodeReader()->GetBuffer();
    //     // publish order request.
    //     g_mqttClient.publish(MQTT_TOPIC_OF_ORDER_REQUEST, 2, true, order_id);
    // }

}

#endif
