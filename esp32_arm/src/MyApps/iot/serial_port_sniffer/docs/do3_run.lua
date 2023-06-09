waitTicks = 0 
function run()
    local evt = richon.WaitEvent() 
    if evt == 0 then 
        local dt = waitTicks 
        waitTicks = richon.GetTicks() 
        dt = waitTicks - dt 
        TaskManager.update(dt) 
    end
    if (evt & 0x01) == 1 then 
        TaskManager.signal('IOCHANGED') 
        richon.ResumeTask(IOTASK) 
    end
    if (evt & 0x02) == 2 then 
        TaskManager.signal('RFRECV') 
    end
    if (evt & 0x04) == 4 then 
        TaskManager.signal('UARTRECV') 
        richon.ResumeTask(UARTTASK) 
    end 
end 


