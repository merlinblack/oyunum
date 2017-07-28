require 'gui/widget'

class 'Button' (Widget)

function Button:__init( atlas, rl, x, y, w )

    Widget.__init( self, x, y, 48, 36 )

    self.state = "normal"
    self.atlas = atlas
    self.bitmaps = {}
    for i = 1, 3 do
        self.bitmaps[i] = Bitmap()
        self.bitmaps[i].x = x + i * 16 - 16
        self.bitmaps[i].y = y
        rl:add( self.bitmaps[i] )
    end
    self.rl = rl

    self:updateVisualState()

end

function Button:destroy()
    for i = 1, 3 do
        rl:remove( self.bitmaps[i] )
    end

    Widget:destroy(self)
end

function Button:move( x, y )
    if Widget.move( self, x, y ) then
        return true
    end

    for i = 1, 3 do
        self.bitmaps[i].x = self.left
        self.bitmaps[i].y = self.top
    end
end

function Button:mouseDown( time, x, y, button )
    if button == 1 and self.state == "hover" then
        self.state = "pressed"
    end
    return self:updateVisualState()
end

function Button:mouseUp( time, x, y, button )
    if button == 1 and self.state == "pressed" then
        self.state = "hover"
    end
    return self:updateVisualState()
end

function Button:mouseMoved( time, x, y, buttons )

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

function Button:updateVisualState()
    if self.state ~= self.oldstate then
        self.bitmaps[1]:setBitmap( self.atlas:getFrame( 'btn_left_' .. self.state ) )
        self.bitmaps[2]:setBitmap( self.atlas:getFrame( 'btn_middle_' .. self.state ) )
        self.bitmaps[3]:setBitmap( self.atlas:getFrame( 'btn_right_' .. self.state ) )
        self.oldstate = self.state
        return true -- state has changed
    end
end

function Button:lostMouse()
    self.state = "normal"
    self:updateVisualState()

    Widget.lostMouse(self)
end
