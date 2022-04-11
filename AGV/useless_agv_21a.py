#  An AGV Control DC Motor wheels
#   Sensors:
#       Track sensor
#       Obstacle sensor
#       RFID reader
#   output:
#       Dual DC motor wheels, with driver L298N
#   Communication
#       UART to Reprap controller. 
#           With gcode
#       Wifi to Simple Scheduling software.
#           With MQTT
#  

def setup_rasp_pi_gpio():
    import RPi.GPIO as GPIO
    PIN_A = 11
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(PIN_A, GPIO.OUT)
#    GPIO.output(PIN_A,True)
    pwm_frequency_hz = 100
    pwm_duty = 50
    my_pwm = GPIO.PWM(PIN_A, pwm_frequency_hz)
    my_pwm.start(0)

    my_pwm.ChangeFrequency(pwm_frequency_hz)
    my_pwm.ChangeDutyCycle(pwm_duty)

    my_pwm.stop()
    GPIO.cleanup()



class DCMotorWheel:
    def __init__(self, pin_a, pin_b, pin_enable) -> None:
        # set direction on GPIO
        self.pin_a = pin_a
        self.pin_b = pin_b
        self.pin_enable = pin_enable

        GPIO.setup(pin_a, GPIO.OUT)
        GPIO.setup(pin_b, GPIO.OUT)
        GOIO.setup(pin_enable, GPIO.OUT)
        GPIO.output(pin_a, True)
        GPIO.output(pin_b, False)
        pwm_frequency_hz = 100
        self.pwm_output = GPIO.PWM(pin_enable, pwm_frequency_hz)
        self.pwm_output.start(0)
        

    def OutputSpeed(self, newSpeed_in_percent) -> None:
        self.pwm_output.ChangeDutyCycle(newSpeed_in_percent)


class PIDController:
    def __init__(self) -> None:
        self.p = 1.123

    def SpinOnce(self, feedback_error:int)->int:
        return self.p * feedback_error


class TrackFollower:
    class TrackSensor:
        def __init__(self) -> None:
            self.pins = [1,2,3,4,5,6,7,8]
            self.dots=[0,0,0,0,0,0,0,0]
            for pin in self.pins:
                GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
            self.last_error = 0

        def __ReadRawTrack(self, follow_right_side:bool) -> tuple:
            # Read from GPIO
            index = 0
            for pin in self.pins:
                self.dots[index] = GPIO.input(pin)
                index += 1

            # Calculat TrackStartIndex, and TrackWidth
            TrackStartIndex = -1
            TrackWidth = 0
            last_bit = False
            bit_index = 0
            for bit in self.dots:
                # this_bit = (the_byte >> bit) & 0x01
                if bit:
                    #  Got a True bit
                    TrackWidth +=1
                    if TrackStartIndex < 0:
                        # Never got start_bit before
                        TrackStartIndex = bit_index
                elif last_bit:
                    # last_bit_value == True, and  this_bit == False
                    return TrackStartIndex, TrackWidth
                last_bit = bit
            return -1, 0  # Can not detect track


        def ReadPositionError(self, follow_right_side:bool) -> int:
            track_start, track_width =  self.__ReadRawTrack()
            # Recalulate Track center
            positions = [-9,-6,-3,-1,1,3,6,9]
            error_sum = 0
            if track_width == 0:
                return self.last_error
            for i in range[track_width]:
                error_sum  += positions[track_start+i]
            error_average = error_sum / track_width
            self.last_error = error_average
            return error_average
            
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
        self.left_wheel = DCMotorWheel(1, 2,pin_enable=3)
        self.right_wheel = DCMotorWheel(4, 5, pin_enable=6)

    def StartMoving(self, target_speed:int):
        self.target_speed = target_speed

    def Stop(self):
        self.target_speed = 0
        self.left_wheel.OutputSpeed(0)
        self.right_wheel.OutputSpeed(0)

    
    def SpinOnce(self):
        feedback = self.TrackSensor.ReadPositionError()
        correcting = 1.234 * self.pid_controller.SpinOnce(feedback)
        self.left_wheel.OutputSpeed = self.target_speed + correcting
        self.right_wheel.OutputSpeed = self.target_speed - correcting