#  An AGV Control DC Motor wheels
#   Sensors:
#       Track sensor
#       Obstacle sensor
#       RFID reader
#   Communication
#       UART to Reprap controller. 
#           With gcode
#       Wifi to Simple Scheduling software.
#           With MQTT
#  


class PIDController:
    def __init__(self) -> None:
        pass

    def SpinOnce(self, feedback_error:int)->int:
        return 0

class DCMotorWheel:
    def __init__(self) -> None:
        pass

    def OutputSpeed(self, newSpeed) -> None:
        pass

class FollowTrackController:
    class TrackSensor:
        def __init__(self) -> None:
            pass

        def ReadPositionError(self) -> int:
            return 0

    class ObstacleSensor:
        def __init__(self) -> None:
            pass

    class RFIDReader:
        def __init__(self) -> None:
            pass


    def __init__(self) -> None:
        # self.is_running = False
        self.current_speed = 0
        self.target_speed = 0
        self.pid_controller = PIDController()
        self.left_wheel = DCMotorWheel()
        self.right_wheel = DCMotorWheel()

    def StartMoving(self, target_speed:int):
        self.target_speed = target_speed

    def Stop(self):
        self.target_speed = 0

    
    def SpinOnce(self):
        feedback = self.TrackSensor.ReadPositionError()
        correcting = 1.234 * self.pid_controller.SpinOnce(feedback)
        self.left_wheel.OutputSpeed = self.target_speed + correcting
        self.right_wheel.OutputSpeed = self.target_speed - correcting


class GarmentBot:
    class STATE:
        IDLE = 0,
        MOVING_TO_CHARGER = 4,
        BATTERY_CHARGING = 1,
        MOVING_TO_SOURCE_SITE = 2,
        MOVING_TO_TARGET_SITE = 3,
        LOADING_BOX = 5,
        UNLOADING_BOX = 6,


    def __init__(self) -> None:
        self.State = self.STATE.IDLE
        self.track_follower = FollowTrackController()



    def onIdle(self):
        pass

    def onMovingToCharger(self):
        self.track_follower.SpinOnce()

    def onBatteryCharging(self):
        pass

    def onMovingToSourceSite(self):
        self.track_follower.SpinOnce()

    def onMovingToTargetSite(self):
        self.track_follower.SpinOnce()

    def onLoadingBox(self):
        pass

    def onUnloadingBox(self):
        pass

    def SpinOnce(self):
        state_tables ={self.STATE.IDLE:self.onIdle, 
                    self.STATE.BATTERY_CHARGING:self.onBatteryCharging,
                    }
        do_something = state_tables[self.State]
        do_something()



mybot = GarmentBot()
while True:
    mybot.SpinOnce()


