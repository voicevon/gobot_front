#include <stdint.h>
#include "HouseMap.h"
// #include "Robot/Commu/CommuUart.h"
// #include "Robot/Commu/CommuBleGattServer.h"
#include "actions.h"
#include "Robot/command_queue.h"

class GobotHouse{
    public:
        static GobotHouse& getInstance()
        {
            static GobotHouse instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }    
        void Setup(RobotAction* pAction, int segments);
        void MoveStone_FromRoomToHead(uint8_t house_id);
        void MoveStone_FromHeadToRoom(uint8_t house_id);


    private:
        void __Move_fromRoom_toDoor(uint8_t house_id, bool forwarding);
        void __Move_fromHead_toNeck(bool forwarding);
        void __Move_fromNeck_toDoor(uint8_t house_id, bool forwarding);
        void __Enable_eefCoil(bool enable);

        HouseMap __map;
        RobotAction* __house_action;
        int __segments;
        CommandQueue* _commandQueue;

};