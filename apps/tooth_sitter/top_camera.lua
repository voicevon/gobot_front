function Test_take_picture()
    local test_topic = "twh/sitter/take_picture/image/2325/1"
    for i=0, 9 do
        size = app.TakePicture()
        print("taking picture",  i, "size=", size, "topic=", test_topic)
        app.PublishPicture(test_topic)
        ri.Delay(3000)
    end
end

function onTimer()
end

function onMqttRx(subscriber_id)
    print('\nonMqttRx')
    if subscriber_id == sub_id then
        payload = app.MqttReadPayload(sub_id)
        print("rx payload=", payload)
        if payload == my_id then
            size = app.TakePicture()
            app.PublishPicture(pub_topic)
            print("published picture, topic=", pub_topic)
        end
    end
end

function main()
	last_event = 0
	while true do
		event = ri.WaitEvent(255)
		if event == 0 then
			onTimer()
		elseif event <=63   then
			onMqttRx(event)
		end
		if (event & 64) == 64 then
			onMasterRx()
		end
		if (event & 128) == 128 then
			onSlaveRx()
		end
	end
end

function setup()
    ri.SetPrint(2)
    -- print(ri.Help())
    -- print(app.Help())
    my_id = '2325/top'
    pub_topic = "twh/sitter/image/" .. my_id
    sub_topic = "twh/sitter/take_picture/command"
    sub_id =app.MqttSubscribe(sub_topic)
    print("subscriber id=", sub_id)
end

setup()
-- Test_take_picture()
main()
-- test_http_print()



