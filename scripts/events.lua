-- events.lua
--

info.mouseButtons = {}
info.mouseAxes = {}

update = coroutine.wrap( function( time )
    local prev = 0
    while( true ) do
        if time - prev > 1 then
            prev = time
            hello.text, world.text = world.text, hello.text
        end
        tick( pulse, time )
        tick( slide, time )
        tick( animate, time )
        coroutine.yield();
    end
end)

function keyEvent( time, keycode, char )
    print( keycode, char, utf8.char(char) )
end

function mouseEvent( time, what, button, x, y, z, dx, dy, dz )
    if what == 'move' then
        info.mouseAxes = { x = x, y = y, z = z, dx = dx, dy = dy, dz = dz }
        mouseMove( x, y )
    end

    if what == 'down' then
        info.mouseButtons[button] = { time = time, x = info.mouseAxes.x, y = info.mouseAxes.y }
    end

    if what == 'up' then
        local details = info.mouseButtons[button]
        local duration = time - details.time
        if duration < .5 then
            mouseClick( button, details.time, details.x, details.y )
        end
    end
end

function mouseClick( button, time, x, y )
end

function mouseMove( x, y )
end
