local coroutine = coroutine 
function coroutine.xpresume(co) 
	local succeed, result = coroutine.resume(co) 
	if not succeed then 
		richon.Printf('LUA Error: ' .. tostring(result) .. '\\n\\r') 
	end
	return succeed 
end 

local table = table 
TaskManager = {} 
local waitingTasks = {} 
local signals = {} 

function TaskManager.update(dt) 
	local waking = {} 
	for co, t in pairs(waitingTasks) do 
		if t.time and t.time > 0 then 
			t.time = t.time - dt 
			if t.time <= 0 then 
				table.insert(waking, co) 
			end
		elseif t.func then 
			if t.func() then
				table.insert(waking, co)
			end
		end
	end
	for _, co in ipairs(waking) do
		waitingTasks[co] = nil 
		coroutine.xpresume(co) 
	end 
end

function TaskManager.wait(cond) 
	local co, result = coroutine.running() 
	if result == 1 then 
		richon.Printf('wait functions can only be called in task' .. '\\r\\n') 
		assert(co, 'wait functions can only be called in task\\r\\n') 
	end 
	if type(cond) == 'number' then 
		waitingTasks[co] = {time = cond * 400 / 1000} 
	elseif type(cond) == 'function' then 
		waitingTasks[co] = { func = cond } 
	elseif type(cond) == 'string' then 
		if signals[cond] == nil then 
			signals[cond] = { co } 
		else 
			table.insert(signals[cond], co) 
		end
	end
	return coroutine.yield() 
end	

function TaskManager.signal(sig) 
	local waitingSignalTasks = signals[sig] 
	if waitingSignalTasks then 
		signals[sig] = nil 
		for _, co in ipairs(waitingSignalTasks) do 
			coroutine.xpresume(co) 
		end 
	end 
end 

function TaskManager.run(func) 
	local co = coroutine.create(func) 
	return coroutine.xpresume(co) 
end 

function TaskManager.kill(co) 
	waitingTasks[co] = nil 
end 