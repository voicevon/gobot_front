function setup()
    print("setup() is here.")
end

p=0 i=0 

function reached_timer()
    now = hw.millis()
    if now - p < 1000  then return false end
    p = now 
    return true
end

function test()
    if not reached_timer() return end
    i=i+1  print(i)
end

function main()
    if a>100 then print(a)
    else 
        if  wd.buttonIsPressed() then
            wd.motorSetSpeed(100)
        end
    end
end



function test_angle()
    a = wd.readEncoderAngle()   print(a)
end

function test_button()
   i=i+1
    bb = wd.readButton()    print(i,bb)
end

speed = 10
function test_motor()
    speed = speed + 25
    print(speed)
    if speed >255 then speed=0 end
    wd.setMotorSpeed(256 - speed)
end

function test_read_second()
    print(i, wd.readDelaySecond())
end

function loop ()
    if not reached_timer() then return end
    -- test_read_second()
    test_button()
end

setup()