function DummyScript() 
    while true do 
        richon.Printf('Dummy\\n\\r') 
        TaskManager.wait(1000) 
        richon.DILEDFlash() 
        richon.UARTLEDFlash() 
    end 
end 

TaskManager.run(DummyScript)
