

//  Known sub class
//       Any robot_hardware who run Gcode
//       Any Agv who run on map.
class MessageConsumer{
    public:
        virtual void FeedMessage(char* message, int length);
};