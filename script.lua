print 'Lua Script'

prev = 0

function update( t )
    if t - prev > 1 then
        prev = t
        hello.text, world.text = world.text, hello.text
    end
end

hello = Text( "Hello", fixed_font, 100, 100 )
world = Text( "World", fixed_font, 120, 120 )
print( hello )
print( fixed_font )

renderlist:add( hello )
renderlist:add( world )

print( 'Completed' );
