doubleTab = doubleTab or {}

keywords = keywords or { 
    ['and'] = 1,       ['break'] = 1,     ['do'] = 1,        ['else'] = 1,      ['elseif'] = 1,    ['end'] = 1,
    ['false'] = 1,     ['for'] = 1,       ['function'] = 1,  ['goto'] = 1,      ['if'] = 1,        ['in'] = 1,
    ['local'] = 1,     ['nil'] = 1,       ['not'] = 1,       ['or'] = 1,        ['repeat'] = 1,    ['return'] = 1,
    ['then'] = 1,      ['true'] = 1,      ['until'] = 1,     ['while'] = 1,
 }

function autoComplete( str )
    local prefixend = string.find( str:reverse(), '[() %[%]=+/,%%]' )
    local prefix = ''
    local possibles

    if prefixend then
        prefix = string.sub( str, 1, #str - prefixend + 1 )
        str = string.sub( str, #str - prefixend + 2 )
    end

    str, possibles = complete(str)

    if #possibles > 1 then
        if doubleTab.str == str then
            print( table.unpack( possibles ) )
        else
            doubleTab.str = str
        end
    end

    return prefix..str
end

function autoCompleteClear()
    doubleTab.str = nil
end

function complete( str )
    local possibles = getCompletions( keywords, str )
    for k,v in pairs( getCompletions( _G, str ) ) do
        table.insert( possibles, v )
    end
    if #possibles > 0 then
        str = string.sub( possibles[1], 1, getIdenticalPrefixLength( possibles, #str ) )
    end
    return str, possibles
end

function getCompletions( where, str )
    local g = where
    local ret = {}
    local dotpos = string.find( str:reverse(), '[%.:]' )
    local prefix = ''
    local dottype = ''

    if dotpos ~= nil then
        dotpos = #str - dotpos
        prefix = string.sub( str, 1, dotpos )
        dottype = string.sub( str, dotpos + 1, dotpos + 1 )
        g = getTable( where, prefix)
        str = string.sub( str, dotpos + 2 )
    end

    if g == nil then
        return {}
    end

    if type( g ) == 'table' then
        for k,v in pairs(g) do
            if string.find( k, str ) == 1 and string.sub(k,1,1) ~= '_' then
                table.insert( ret, prefix .. dottype .. k )
            end
        end
        if g.__luaclass then
            for k, v in pairs( getLuaClassInfo( g ) ) do
                if string.find( v, str ) == 1 then
                    table.insert( ret, prefix .. dottype .. v )
                end
            end
        end
    else
        -- Retrieve class info if any
        for k,v in pairs( getClassInfo( g ) ) do
            if string.find( v, str ) == 1 then
                table.insert( ret, prefix .. dottype .. v )
            end
        end

    end

    return ret
end

function getTable( where, tblname )
    --print( 'Looking up:', tblname )
    local lastdot = string.find( tblname:reverse(), '%.' )
    if lastdot == nil then
        return where[tblname]
    end
    local prefix = string.sub( tblname, 1, #tblname - lastdot )
    local tbl = getTable( where, prefix )
    local subscript = string.sub( tblname, #tblname - string.find( tblname:reverse(), '%.' ) + 2 )
    --print( "Subscript:", subscript, tblname )
    return tbl[subscript]
end

function getIdenticalPrefixLength( tbl, start )
    if #tbl == 0 then return start end
    local l = start
    local str
    local allSame = true
    while allSame == true do
        if l > #tbl[1] then return #tbl[1] end
        str = string.sub( tbl[1], 1, l )
        for k, v in pairs( tbl ) do
            if string.find( v, str ) ~= 1 then
                allSame = false
            end
        end
        l = l + 1
    end
    return l - 2
end

function getClassInfo( cls )
    local ret = {}
    local mt = getmetatable( cls )
    if mt then
        for k, v in pairs( mt ) do
            if string.sub( k, 1, 1 ) ~= '_' then
                table.insert( ret, k )
            end
        end
        if mt.__properties then
            for k, v in pairs( mt.__properties ) do
                if string.sub( k, 1, 1 ) ~= '_' then
                    table.insert( ret, k )
                end
            end
        end
    end

    return ret
end

function getLuaClassInfo( cls )
    local ret = {}
    local mt = getmetatable( cls )
    if mt then
        if mt.__index then
            for k, v in pairs( mt.__index ) do
                if string.sub( k, 1, 1 ) ~= '_' then
                    table.insert( ret, k )
                end
            end
        end
    end
    return ret
end

