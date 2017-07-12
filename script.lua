print 'Lua Script'

prev = 0

function update( t )
    if t - prev > 1 then
        prev = t
        hello.text, world.text = world.text, hello.text
    end
    tick( pulse, t )
    tick( slide, t )
    tick( run_animate, t )
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

idle_animate = coroutine.create( function( time )
    local prev = time
    while true do
        for index=1,10 do
            while time - prev < .20 do
                time = coroutine.yield()
            end
            prev = time
            indy:setBitmap( Idle[index] )
        end
    end
end )

run_animate = coroutine.create( function( time )
    local prev = time
    while true do
        for index=1,10 do
            while time - prev < .08 do
                time = coroutine.yield()
            end
            prev = time
            indy:setBitmap( Run[index] )
        end
    end
end )

ts = TileSet()
ts:loadSourceBitmap( 'data/dawn_of_the_gods.png' );
print(ts)

package.path = './data/?.lua;' .. package.path
map = require( 'map' )
layer1 = map.layers[1]

tg = TileGrid( ts, layer1.width, layer1.height )
tg:setData( layer1.data )
tg:setXY( 0, 0 )

renderlist:insert( tg )

print( AllegroColor )
white = AllegroColor( 255, 255, 255, 255 )
grey  = AllegroColor( 128, 128, 128, 255 )
print( grey )

hello = Text( "Hello", fixed_font, white, 100, 100 )
world = Text( "World", fixed_font, grey,  120, 120 )

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

Idle = {}
Idle[1] = Bitmap( 'data/sprites/templerun/Idle_0.png' );
Idle[2] = Bitmap( 'data/sprites/templerun/Idle_1.png' );
Idle[3] = Bitmap( 'data/sprites/templerun/Idle_2.png' );
Idle[4] = Bitmap( 'data/sprites/templerun/Idle_3.png' );
Idle[5] = Bitmap( 'data/sprites/templerun/Idle_4.png' );
Idle[6] = Bitmap( 'data/sprites/templerun/Idle_5.png' );
Idle[7] = Bitmap( 'data/sprites/templerun/Idle_6.png' );
Idle[8] = Bitmap( 'data/sprites/templerun/Idle_7.png' );
Idle[9] = Bitmap( 'data/sprites/templerun/Idle_8.png' );
Idle[10] = Bitmap( 'data/sprites/templerun/Idle_9.png' );

Run = {}
Run[1] = Bitmap( 'data/sprites/templerun/Run_0.png' );
Run[2] = Bitmap( 'data/sprites/templerun/Run_1.png' );
Run[3] = Bitmap( 'data/sprites/templerun/Run_2.png' );
Run[4] = Bitmap( 'data/sprites/templerun/Run_3.png' );
Run[5] = Bitmap( 'data/sprites/templerun/Run_4.png' );
Run[6] = Bitmap( 'data/sprites/templerun/Run_5.png' );
Run[7] = Bitmap( 'data/sprites/templerun/Run_6.png' );
Run[8] = Bitmap( 'data/sprites/templerun/Run_7.png' );
Run[9] = Bitmap( 'data/sprites/templerun/Run_8.png' );
Run[10] = Bitmap( 'data/sprites/templerun/Run_9.png' );

indy = Bitmap()
indy:setBitmap( Idle[1] )
indy.x = 260
indy.y = 215
indy.scale = .15

renderlist:add( indy )

print( 'Completed' );
