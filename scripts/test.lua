function getTileMap( tileset, mapdata )
    local tg = TileGrid( tileset, mapdata.width, mapdata.height )
    tg:setData( mapdata.data )
    tg:setXY( 0, 0 )
    return tg
end


map = dofile 'data/magic.lua'

ts = TileSet(1,16,16)
tsdata = map.tilesets[1]
ts:loadSourceBitmap( 'data/' .. tsdata.image )
back1  = getTileMap( ts, map.layers[1] )
front2 = getTileMap( ts, map.layers[2] )
front1 = getTileMap( ts, map.layers[3] )

renderlist:add( back1 )
renderlist:add( front2 )
renderlist:add( front1 )

function movelevel( x, y )
    back1:setXY( x, y )
    front2:setXY( x, y )
    front1:setXY( x, y )
end
