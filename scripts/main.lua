print 'Lua Script'
package.path = './scripts/?.lua;' .. package.path
require( 'sprite' )
require( 'tasks' )
map_data = dozipfile( './data/map.lua.gz' )

prev = 0

function update( t )
    if t - prev > 1 then
        prev = t
        hello.text, world.text = world.text, hello.text
    end
    tick( pulse, t )
    tick( slide, t )
    tick( animate, t )
end

function keyEvent( code, char )
    print( code, char, utf8.char(char) )
end

function mouseEvent( what, button, x, y, z, dx, dy, dz )
    if what ~= 'move' then
        print( what, button, x, y, z, dx, dy, dz )
    end
end

function clear()
    console 'clear'
end

function tick( co, t )
    ret, error = coroutine.resume( co, t )
    if not ret and error ~= 'cannot resume dead coroutine' then 
        print( error )
    end
end

pulse = coroutine.create( function()
    while true do
        for i=0,255,5 do
            c = world.color
            c.r = i
            world.color = c
            coroutine.yield()
        end
        for i=255,0,-5 do
            c = world.color
            c.r = i
            world.color = c
            tile1.x = 16 + i//10
            tile2.x = 48 + i//10
            coroutine.yield()
        end
    end
end )

slide = coroutine.create( function()
    while true do
        for i=16, 400, 1 do
            tile1.x = i
            tile2.x = 32 + i
            coroutine.yield()
        end
        for i=400, 16, -1 do
            tile1.x = i
            tile2.x = 32 + i
            coroutine.yield()
        end
    end
end )

animate = coroutine.create( function( time )
    local prev = time
    while true do
        for index=0,9 do
            while time - prev < .08 do
                time = coroutine.yield()
            end
            prev = time
            indy:setBitmap( indy_sprite:getframe( 'Run_' .. index ) )
        end
    end
end )

ts = TileSet()
ts:loadSourceBitmap( 'data/dawn_of_the_gods.png' );
print(ts)

layer1 = map_data.layers[1]

tg = TileGrid( ts, layer1.width, layer1.height )
tg:setData( layer1.data )
tg:setXY( 0, 0 )

renderlist:insert( tg )

print( AllegroColor )
white = AllegroColor( 255, 255, 255, 255 )
grey  = AllegroColor( 128, 128, 128, 255 )
print( grey )

ttf_font = Font( 'data/6809-chargen/6809 chargen.ttf', -32 );

if not ttf_font then
    print( "It didn't load!!" )
    ttf_font = fixed_font
else
    print( "Yay!" )
end

hello = Text( "Günydin", ttf_font, white, 100, 100 )
world = Text( "Hola", ttf_font, grey,  130, 130 )

print( hello )
print( fixed_font )

renderlist:add( hello )
renderlist:add( world )

tile1 = ts:getTile( 44 )
tile2 = ts:getTile( 47 )

tile1.x, tile1.y = 16, 170
tile2.x, tile2.y = 48, 170

renderlist:add( tile1 )
renderlist:add( tile2 )

print( tile1 )

indy_sprite = Sprite( 'data/sprites/templerun' )

indy = Bitmap()
indy.x = 260
indy.y = 200

renderlist:add( indy )

print( 'Completed' );
