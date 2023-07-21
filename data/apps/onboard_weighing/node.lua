
function onTimer(now)
    if now - started_at > 1000 then
        -- local distance = app.ReadDistance()
        local distance = 1111
        app.Write(my_node_id, distance)
        started_at = now
        print("onTimer() start_at", started_at)
    end
end

function onReceive(now)
    local node_id, distance = app.Read()
    print("on Received()   node_id, my_following_node_id, distance", node_id, my_following_node_id, distance)
    if node_id == my_following_node_id then
        -- distance = app.ReadDistance()
        r.Delay(100)
        distance = 5555
        app.Write(my_node_id, distance)
        started_at = now
        print("onReceive() start_at", started_at)
    end
end

function setup()
    my_node_id = 4
    my_following_node_id = my_node_id - 1
    if my_following_node_id == 0 then
        my_following_node_id = 4
    end

    started_at = 0
end


function loop()
    while (true) do
        local event = r.WaitEvent()
        local now = r.Millis()
        if (event == 0) then
            onTimer(now)
        elseif (event & 1) == 1 then
            onReceive(now)
        end
    end
end

print('---------------------------OnBoard Weighing, running  ----------------------------------')
setup()
loop()

