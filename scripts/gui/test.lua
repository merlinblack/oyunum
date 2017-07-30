require 'gui/widget'

class 'DragButton' (Widget)

function DragButton:__init( atlas, rl, x, y )

    Widget.__init( self, x, y, 48, 48 )

    self.state = "normal"
    self.oldstate = "normal"
    self.atlas = atlas
    self.bitmap = Bitmap()
    self.bitmap:setBitmap( self.atlas:getFrame( 'dragbutton_' .. self.state ) )
    self.bitmap.x = x
    self.bitmap.y = y
    self.rl = rl

    rl:add( self.bitmap )

end

function DragButton:destroy()
    self.rl:remove( self.bitmap )

    Widget:destroy(self)
end

function DragButton:move( x, y )
    if Widget.move( self, x, y ) then
        return true
    end

    -- Needs to be an integer...
    self.bitmap.x = math.floor(self.left)
    self.bitmap.y = math.floor(self.top)
end

function DragButton:mouseDown( time, x, y, button )
    if button == 1 and self.state == "hover" then
        self.state = "pressed"
    end
    return self:updateVisualState()
end

function DragButton:mouseUp( time, x, y, button )
    if button == 1 and self.state == "pressed" then
        self.state = "hover"
    end
    return self:updateVisualState()
end

function DragButton:mouseMoved( time, x, y, buttons )

    -- Update possition BEFORE checking if we have lost the mouse
    if self.state == "pressed" then
        self:move( info.mouseAxes.dx, info.mouseAxes.dy )
    end

    Widget.mouseMoved( self, time, x, y, buttons )

    if self:intersects( x, y ) then
        if self.state == "normal" then
            self.state = "hover"
        end
    else
        self.state = "normal"
    end

    return self:updateVisualState()
end

function DragButton:updateVisualState()
    if self.state ~= self.oldstate then
        self.bitmap:setBitmap( self.atlas:getFrame( 'dragbutton_' .. self.state ) )
        self.oldstate = self.state
        return true -- state has changed
    end
end

function DragButton:lostMouse()
    self.state = "normal"
    self:updateVisualState()

    Widget.lostMouse(self)
end
