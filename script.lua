print 'Lua Script'

prev = 0

function update( t )
    if t - prev > 1 then
        prev = t
        hello.text, world.text = world.text, hello.text
    end
    ret, error = coroutine.resume( pulse )
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
            tile1.x = 16 + i//10
            tile2.x = 48 + i//10
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

tiles = Bitmap()
tiles:loadFile( 'data/dawn_of_the_gods.png' );

tile1 = tiles:getSubBitmap( 32, 32, 32, 32 );
tile2 = tiles:getSubBitmap( 64, 32, 32, 32 );

tile1.x, tile1.y = 16, 170
tile2.x, tile2.y = 48, 170

renderlist:add( tile1 )
renderlist:add( tile2 )

print( tile1 )

print( 'Completed' );
