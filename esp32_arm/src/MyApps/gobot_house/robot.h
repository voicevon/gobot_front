#pragma once


#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"
#include  "MyApps/gobot_house/HouseMap.h"

class GobotHouse_Robot: public GcodeProducer, public MqttMessageConsumer{
    public:
        enum SITE_TYPE{
            HEAD,
            NECK,
            DOOR,
            ROOM,
        };
        static GobotHouse_Robot& getInstance()
        {
            static GobotHouse_Robot instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }    
        // void Setup(RobotAction* pAction);
        void Setup();
        void SpinOnce();
        void ParkArms(bool do_homing);
        void Calibrate(int step, bool enable_eef_coil);
        // bool MoveStone_FromRoomToHead(uint8_t house_id);
        void Test_Alpha(int loop_count);
        void Test_Beta(int loop_count);
        void Test_FollowJig(int loop_count);
        void Test_MoveStone_FromRoomToHead(int loop_count, uint8_t room_index);
        void Test_MoveStone_FromHeadToRoom(int loop_count, uint8_t house_id);
        void __Home();

    private:
        void __MakeGcode_and_Send(FkPosition_XY* from, FkPosition_XY* to, int segment_count);
        void AsyncExecuteMqttCommand(const char* command) override;
        void __Move_fromRoom_toGate(uint8_t room_id, bool forwarding);
        // void __Move_fromRoom_toDoor(uint8_t room_id, bool forwarding);
        void __Move_fromHead_toNeck(bool forwarding);
        void __Move_fromNeck_toGate(uint8_t room_id, bool forwarding);
        void __Move_fromParking_toNeck();
        void __Move_fromParking_toDoor(uint8_t door_id);
        void __Enable_eefCoil(bool enable);
        void __Pause(uint8_t second);
        void __PreHome();

        HouseMap __map;
        // RobotAction* __house_action;
        int __segments;


};