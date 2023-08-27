--  Tooth Acupuncture-2024,   test-jig-A
--  Mac_addr= 180_230_45_175_147_229
function onTimer()
end

function onMqttRx(subscriber_id)
  print('\nonMqttRx ')
  if subscriber_id == sub_id then
      payload = mqtt.ReadPayload(sub_id)
      print("rx payload=", payload)   
      robot.AppendGcode(payload)
  end
end


function loop()
	while true do
		event = mcu.OnEvent(255)
		if event == 0 then
			onTimer()
		elseif event <=7   then
			onMqttRx(event)
		end
		if (event & 128) == 128 then
			onCncArrived()
		end
	end
end

function setup()
    i2c.Begin()
    i2c.Scan()
    app.Begin()

end




print(i2c.Help())
setup()

--loop()



