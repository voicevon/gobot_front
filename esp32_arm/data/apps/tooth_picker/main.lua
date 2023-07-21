last_timer = 0
state = "not_homed"
do_pickup_count = 0

function do_mqtt_message()
    msg_payload = app.ReadMqttMessage()
    if msg_payload == 'pickup' then 
        pick_up()
    end
    if msg_payload == 'placedown' then
        place_down()
    end
end

function onGcodeFinished()

    if state == "parking" then
        robot.AppendGcode("G1X10Y10")
        state = "parked"
    elseif state == "going_down_b" then
        robot.AppendGcode("G1X10Y10")
        state = "parking_failed"
    elseif state == "going_up_a" then
        robot.AppendGcode("G1X10Y20")
        state = "aligning"
    elseif state == "aligning" then
        -- robot.AppendGcode("G1X10Y10")
        last_timer = now
        state = "delaying"
    elseif state == "parked" then
    else
        print("unknown state", state)
    end

end

--  *******************************************************************
function onTimer(now)
    if state == "parked" then
        if do_pickup_count >0 then
            do_pickup_count = do_pickup_count -1
            robot.AppendGcode("G1X30Y30")
            board.SetInnerVacuum(1)
            state= "going_down_b"          
        end
    elseif state == "not_homed" then
        if robot.IsHomed() then
            robot.AppendGcode("G1X10Y10")
            state = "parking"
        end
    elseif state == "going_down_b" then
        air_p = board.ReadAirPressure()
        if air_p < 1.234 then
            print("------------------------------------------------   low air pressure detected, suck tooth ok.")
            robot.AppendGcode("G1X10Y20")
            state = "going_up_a"
        end
    elseif state == "delaying" then
        if now - last_timer > 600 then
            print("------------------------------------------------  released tooth, drop it down")
            board.SetInnerVacuum(0)
            robot.AppendGcode("G1X10Y10")
            state = "parking"
        else
            print("delaying...")
        end
    elseif state == "parked" then
    elseif state == "parked_failed" then
    elseif state == "parking" then
    elseif state == "going_up_a" then
    else 
        print("on Timer unknown state================",state)
    end
end

function loop()
    for i=1, 99999999, 1 do
        now = r.Millis()
        
        local event = r.WaitEvent();
        -- print("event=", event)
        if (event == 0) then
            onTimer(now)
        elseif (event & 1) == 1 then
            print("event_bit = 0")
            -- do_mqtt_message()
        elseif (event & 2) == 2 then
            -- EVENT_BIT_HAS_NO_MOVEMENT_ACTUATORS
            onGcodeFinished()
        else
            print ("unknown event", event)
        end
    end
end


function setup()
    print('-------------------------------------------------------------')
    robot.AppendGcode("G28X")
    robot.AppendGcode("G28Y")  
end

setup()
do_pickup_count = 19
loop()
robot.AppendGcode("G1X10Y20")
print('*******************************************************************')

