map_data = dozipfile 'data/magic.lua.gz'

function getLayer( layer )
    
    for _, v in pairs( map_data.layers ) do
        if v.type == 'tilelayer' then
            if v.name == layer then
                return v
            end
        end
    end

    return nil
end

function getTileGrid( ts, layerName )
    local layer = getLayer( layerName )
    if not layer then
        print( 'Could not find layer: ' .. layerName )
        return nil
    end
    local tg = TileGrid( ts, layer.width, layer.height )
    tg:setData( layer.data )
    tg:setXY( 0, 0 )
    return tg
end

ts = TileSet(1, 16, 16)
if not ts:loadSourceBitmap( 'data/magic_cliff_tileset.png') then
    print( 'Could not load tile set bitmap' )
    return
end

tilegrids = {}
tilegrids.front1 = getTileGrid( ts, 'Front 1' )
tilegrids.front2 = getTileGrid( ts, 'Front 2' )
tilegrids.back1 =  getTileGrid( ts, 'Back 1' )

indy_sprite = Sprite( 'data/sprites/templerun' )

indy = Bitmap()
indy.x = 150
indy.y = 120

renderlist:insert( tilegrids.back1 )
renderlist:insert( indy )
renderlist:insert( tilegrids.front2 )
renderlist:insert( tilegrids.front1 )

animate = coroutine.create( function( time )
    local prev = time
    while true do
        for index=0,9 do
            while time - prev < .08 do
                time = coroutine.yield()
            end
            prev = time
            indy:setBitmap( indy_sprite:getFrame( 'Run_' .. index ) )
        end
    end
end )

slide = coroutine.create( function()
    while true do
        for i=0, 25*32, 1 do
            tilegrids.front1:setXY( -i, 0 )
            tilegrids.front2:setXY( -i, 0 )
            tilegrids.back1:setXY( -i, 0 )
            coroutine.yield()
        end
    end
end )

table.insert( tasks, animate )
table.insert( tasks, slide )

require 'gui/button'
btn_console = Button( gui.atlas, renderlist, 16, 400, 96, "Console" )
function btn_console:action()
    console('toggle')
end
gui:addChild( btn_console )
