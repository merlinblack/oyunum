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

function valuesToKeys( t )
    local r = {}
    for k, v in pairs( t ) do
        r[v] = k
    end
    return r
end
