-- events.lua
--

info.mouseButtons = {}
info.mouseButton = 0
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
        time = coroutine.yield();
    end
end)

function keyEvent( time, keycode, codepoint )
    print( keycode, codepoint, utf8.char(codepoint) )
end

function mouseEvent( time, what, button, x, y, z, dx, dy, dz )
    if what == 'move' then
        info.mouseAxes = { x = x, y = y, z = z, dx = dx, dy = dy, dz = dz }
        mouseMoved( info.mouseButton, time, x, y )
    end

    if what == 'down' then
        info.mouseButtons[button] = { time = time, x = info.mouseAxes.x, y = info.mouseAxes.y }
        info.mouseButton = info.mouseButton | (1 << (button-1))
        mouseDown( button, time, x, y )
    end

    if what == 'up' then
        mouseUp( button, time, x, y )
        local details = info.mouseButtons[button]
        local duration = time - details.time
        if duration < .5 then
            mouseClick( button, details.time, details.x, details.y )
        end
        info.mouseButton = info.mouseButton & ~(1 << (button-1))
    end
end

-- These will be redefined in other code.
function mouseClick( button, time, x, y ) end
function mouseMoved( buttons, time, x, y ) end
function mouseDown( button, time, x, y ) end
function mouseUp( button, time, x, y ) end
