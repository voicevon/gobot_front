#include <stdint.h>
#include "HouseMap.h"
// #include "Robot/Commu/CommuUart.h"
// #include "Robot/Commu/CommuBleGattServer.h"
#include "actions.h"
#include "Robot/command_queue.h"
#include "gobot_house_hw.h"


class GobotHouse{
    public:
        enum SITE_TYPE{
            HEAD,
            NECK,
            DOOR,
            ROOM,
        };
        static GobotHouse& getInstance()
        {
            static GobotHouse instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }    
        void Setup(RobotAction* pAction);
        void SpinOnce();
        void ParkArms(bool do_homing);
        void ParkForCalibration(int step);
        bool MoveStone_FromRoomToHead(uint8_t house_id);
        bool MoveStone_FromHeadToRoom(uint8_t house_id);


    private:
        void __Move_fromRoom_toDoor(uint8_t house_id, bool forwarding);
        void __Move_fromHead_toNeck(bool forwarding);
        void __Move_fromNeck_toDoor(uint8_t house_id, bool forwarding);
        void __Enable_eefCoil(bool enable);

        GobotHouseHardware* __robot_hardware;
        HouseMap __map;
        RobotAction* __house_action;
        int __segments;
        CommandQueue* __commandQueue;

};