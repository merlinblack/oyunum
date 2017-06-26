print 'Lua Script'

prev = 0

function update( t )
    if t - prev > 1 then
        prev = t
        hello.text, world.text = world.text, hello.text
    end
    coroutine.resume( pulse )
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

print( 'Completed' );
