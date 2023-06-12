

p=0 i=0 state = 1 timer=0 origin_angle = 0.1 starting=0 speed=10.1
function reached_timer()
 now = hw.millis()
 if now - p < 1000 then return false end
 p = now return true
end
function test_all()
 if not reached_timer() then return end
 i = i + 1
 a = wd.readEncoderAngle()
 b = wd.readButton()
 t = wd.readDelaySecond()
 print(i, a, b, t, state, hw.millis()-starting, speed)
end

function at_state_idle()
 angle = wd.readEncoderAngle()
 if angle > 10 then 
    origin_angle = angle 
    state = 3 
return end
 if wd.readButton() then
 wd.setMotorSpeed(256 - 256) 
 timer = wd.readDelaySecond() * 1000 
 starting = hw.millis() 
 state = 2 
 return
 end
end

function at_state_run()
 now = hw.millis()
 if now - starting > timer then wd.setMotorSpeed(256 - 0) state = 1 end
end

function at_state_pwm()
 angle = wd.readEncoderAngle() speed = 11.2 * (angle - origin_angle)
 if speed > 255 then speed = 255 end
 if speed < 10 then 
    wd.setMotorSpeed(256 - 0) 
    state = 1 
end
end

function setup()
 print("setup() is here.") wd.setMotorSpeed(256-0) end
function loop ()
 test_all()
 if state == 1 then at_state_idle() end
 if state == 2 then at_state_run() end
 if state == 3 then at_state_pwm() end
end
