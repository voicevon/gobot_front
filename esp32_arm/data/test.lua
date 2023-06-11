function setup()
end

p=0 i=0 
function loop()
  now = hw.millis()
  if now-p <1000  then return end
  p=now  i=i+1  print(i)
  
end