


function onTimer(now)
    if (now - started_at > 800) then
        started_at = r.Millis()
        print("........................node time out", last_node_id)
    end
end

function onReceive(now)
    local node_id, distance = app.Read()
    -- print("node_id, last_node_id, distance", node_id, last_node_id, distance)
    app.SetLed(node_id -1, 1)   -- turn on
    app.SetLed(last_node_id, 0)  -- turn off
    last_node_id = node_id
    started_at = now
    if (distance==1111) then
        print("timeout_node_id, distance", node_id, distance)
    else
        print("-------------normal_node_id, distance", node_id, distance)
    end
end

function setup()
    started_at = 0
    app.SetLed(0, 1)
    last_node_id = 0
end


function loop()
    while true do
        local event = r.WaitEvent()
        local now = r.Millis()
        if (event == 0) then
            onTimer(now)
        elseif (event & 1) == 1 then
            onReceive(now)
        end
    end
end

print('---------------------------OnBoard Weighing head, running ----------------------------------')

setup()
loop()