require 'class'
require 'sprite'

class 'gui'

gui.children = {}
gui.atlas = Sprite 'data/sprites/gui'

function gui:addChild( child )
    -- should check all the methods are present ...
    table.insert( self.children, child )
end

function mouseMoved(b,time,x,y)
    for _,child in pairs( gui.children ) do
        if child:mouseMoved(time, x, y, b) then
            break
        end
    end
end

function mouseClick(b,time,x,y)
    for _,child in pairs( gui.children ) do
        if child:mouseClick(time, x, y, b) then
            break
        end
    end
end

function mouseUp(b,time,x,y)
    for _,child in pairs( gui.children ) do
        if child:mouseUp(time, x, y, b) then
            break
        end
    end
end

function mouseDown(b,time,x,y)
    for _,child in pairs( gui.children ) do
        if child:mouseDown(time, x, y, b) then
            break
        end
    end
end
