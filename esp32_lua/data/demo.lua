local a = 0
local demo = {}
demo.get_a = function()
    a = a + 1
    return a
end

return demo
