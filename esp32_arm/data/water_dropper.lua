p = 0 
i = 0 
state = 1 
timer = 0 
starting = 0 
speed = 10
zero_point = 0.1
MAX_SPEED = 255
MIN_SPEED = 0
logic_speed = 1.1

k_angle = 1.1
k_speed = 5.1

require "math"

function reached_timer()
    now = hw.millis()
    if now - p < 1000 then 
        return false 
    end
    p = now 
    return true
end

function test_all()
    if not reached_timer() then 
        return 
    end
    i = i + 1
    a = wd.readEncoderAngle()
    b = wd.readButton()
    t = wd.readDelaySecond()
    print("i=",i, "angle=",a, "button=", b, t, "state=",state, "zero_point=", zero_point, hw.millis()-starting, speed, logic_speed)
end


function get_angle()
    return  5.1 * (wd.readEncoderAngle() - zero_point)
end

function set_speed()
     wd.setMotorSpeed(Math.floor(MAX_SPEED - logic_speed))
end

function at_state_idle()
    angle = get_angle()
    if angle > 10 then 
        state = 3 
        return 
    end
    if wd.readButton() then
        -- logic_speed = 11.2 * get_angle()
        logic_speed = MAX_SPEED
        timer = wd.readDelaySecond() * 1000
        starting = hw.millis() 
        state = 2 
        return
    end
end

function at_state_run()
    now = hw.millis()
    if now - starting > timer then 
        logic_speed = MIN_SPEED 
        state = 1 
    end
end


function at_state_pwm()
    speed = 5.2 * get_angle()
    if speed > 255 then 
        speed = 255 
    end
    logic_speed = speed;
    if angle < 10 then 
        logic_speed = MIN_SPEED
        state = 1 
    end
end

function setup()
    print("setup() is here.") 
    wd.setMotorSpeed(256-0)
    logic_speed = MIN_SPEED
    zero_point = wd.readEncoderAngle()
end

function loop ()
    test_all()
    if state == 1 then at_state_idle() end
    if state == 2 then at_state_run() end
    if state == 3 then at_state_pwm() end
    set_speed()
end