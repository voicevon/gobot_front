function setup()

end

function loop()
    a = wd.readSensorAngle() 
    if a>100 then
        print(a)
    else
        if  wd.buttonIsPressed() then
            wd.motorSetSpeed(100)
        end
    end
end

p=0 i=0 
function loop2()
  now = hw.millis()
  if now-p <1000  then return end
  p=now  i=i+1  print(i)
  angle = wd.readSensorAngle()
  print(angle)
end