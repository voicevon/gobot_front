#include <stdint.h>
#include "HouseMap.h"
// #include "Robot/Commu/CommuUart.h"
// #include "Robot/Commu/CommuBleGattServer.h"
// #include "actions.h"
#include "Robot/command_queue.h"


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
        // void Setup(RobotAction* pAction);
        void Setup();
        void SpinOnce();
        void ParkArms(bool do_homing);
        void Calibrate(int step, bool enable_eef_coil);
        bool MoveStone_FromRoomToHead(uint8_t house_id);
        bool MoveStone_FromHeadToRoom(uint8_t house_id);
        CommandQueue* GetCommandQueue(){return this->__commandQueue;};


    private:
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
        CommandQueue* __commandQueue;

};