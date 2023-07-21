
state = 1
started_at  = 0
delay_ms = wd.ReadSettingTimer()
angle_ratio = 3.5

function onTimer(now)
    if state == 2 then
        print("timer", now - started_at)
        if now - started_at > delay_ms then
            print("leaving state 2")
            wd.SetSpeed(0)
            state = 1
        end
    end
end

function onButton(button_value)
    if state == 1 then
        if button_value then
            print("button", button_value)
            started_at = r.Millis()
            wd.SetSpeed(255)
            state = 2
        end
    end
end

function onAngle(angle)
    if state == 1 then
        print("state 1 >> angle", angle)
        if angle > 10 then
            state = 3
        end
    end

    if state == 3 then
        print("state 3 >> angle", angle)
        if angle < 5 then
            wd.SetSpeed(0)
            state = 1
        else
            -- wd.SetSpeed(255)
            speed = 0.8 * angle
            if (speed > 255) then
                speed = 255
            end
            wd.SetSpeed(speed)
        end
    end
end


function loop()
    print('---------------------------Water dropper, running loop ----------------------------------')
    while true do
        now = r.Millis()
        event = r.WaitEvent();
        onTimer(now)
        if (event & 1) == 1 then
            onButton(wd.ReadButton())
        end
        if (event & 2) == 2 then
            onAngle(wd.ReadAngle() * angle_ratio)
        end
    end
end

loop()