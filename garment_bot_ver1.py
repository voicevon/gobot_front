from AGV.agv_21a import TrackFollower
import RPi.GPIO as GPIO


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
        GPIO.setmode(GPIO.BOARD)
        self.State = self.STATE.IDLE
        self.track_follower = TrackFollower()



    def onIdle(self):
        # Check task from scheduler
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


