require 'class'

class 'Widget'

function Widget:__init( x, y, w, h )
    self.children = {}
    self.left   = x
    self.top    = y
    self.width  = w
    self.height = h
end

-- Can be called, directly or by garbage collection
-- if garbage collection runs it, it must not error!!
function Widget:destroy()
    if type(self.children) == 'table' then
        for _,child in pairs(self.children) do
            child:destroy()
        end
    end
end

function Widget:intersects( x, y )
    if x < self.left then
        return false
    end
    if y < self.top then
        return false
    end
    if x > self.left + self.width then
        return false
    end
    if y > self.top + self.height then
        return false
    end
    return true
end

function Widget:mouseMoved( x, y, button )
    if self:intersects( x, y ) then
        for _,child in pairs(self.children) do
            if child.mouseMoved then child:mouseMoved( x, y, button ) end
        end
    else
        self:lostMouse()
    end
end

function Widget:keypressed( key )
    for _,child in pairs(self.children) do
        if child.keypressed then child:keypressed( key ) end
    end
end

function Widget:keyreleased( key )
    for _,child in pairs(self.children) do
        if child.keyreleased then child:keyreleased( key ) end
    end
end

function Widget:addChild( widget )
    table.insert(self.children, widget)
end

-- Return true to cancel move, when it would move off view
function Widget:move( x, y )
    local left = self.left
    local top = self.top
    local width = self.width
    local height = self.height

    if left + x < 0 or top + y < 0 then return true end
    if left + width + x > info.width or top + height + y > info.height then
        return true
    end

    self.left = self.left + x
    self.top = self.top + y

    for _,child in pairs( self.children ) do
        if child.move then child:move( x, y ) end
    end
end

function Widget:lostMouse()
    for _,child in pairs( self.children ) do
        if child.lostMouse then child:lostMouse() end
    end
end
