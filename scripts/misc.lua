-- misc.lua
--

function clear()
    console 'clear'
end

function pt( t )
    for k,v in pairs(t) do
        print( k, v )
    end
end

