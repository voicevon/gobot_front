// #include "HouseMap.h"
#include  "MyApps/gobot_house/HouseMap.h"
#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"

class GobotHouse_2206: public GcodeProducer, public MqttMessageConsumer{
    public:
        enum SITE_TYPE{
            HEAD,
            NECK,
            DOOR,
            ROOM,
        };
        static GobotHouse_2206& getInstance()
        {
            static GobotHouse_2206 instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }    
        // void Setup(RobotAction* pAction);
        void Setup();
        void SpinOnce();
        void ParkArms(bool do_homing);
        void Calibrate(int step, bool enable_eef_coil);
        bool MoveStone_FromRoomToHead(uint8_t house_id);
        bool MoveStone_FromHeadToRoom(uint8_t house_id);
        void Test_HomeAlpha(int loop_count);
        void Test_HomeBeta(int loop_count);


    private:
        void ExecuteMqttCommand(const char* command) override;
        void __Move_fromRoom_toDoor(uint8_t house_id, bool forwarding);
        void __Move_fromHead_toNeck(bool forwarding);
        void __Move_fromNeck_toDoor(uint8_t house_id, bool forwarding);
        void __Move_fromParking_toNeck();
        void __Move_fromParking_toDoor(uint8_t door_id);
        void __Enable_eefCoil(bool enable);
        void __Home();
        void __Pause(uint8_t second);
        void __PreHome();

        HouseMap __map;
        // RobotAction* __house_action;
        int __segments;


};