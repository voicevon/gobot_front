function setup()
    pinMode(2,3)
    print ("setup()")
  end
  
  p = 0
  out = 0
  i=0
  function loop()
    -- print("loop() is entering")
    now = millis()
    -- print (i, p, now,out)
    if(now -p) < 1000 then  return end
    print("1000 ms posted")
    if out == 0 then out = 1 else out = 0  end
    print("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")
    digitalWrite(2,out)
    print("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq")
    p = now  
    
  end