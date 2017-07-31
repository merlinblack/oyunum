require 'gui/widget'

class 'Button' (Widget)

Button.font = Font( 'data/6809-chargen/6809 chargen.ttf', -16 );
Button.colors = {}
Button.colors.normal  = AllegroColor( 0,    0,   0, 255 )
Button.colors.hover   = AllegroColor( 0,    0, 255, 255 )
Button.colors.pressed = AllegroColor( 0,    0, 255, 255 )

function Button:__init( atlas, rl, x, y, w, caption )

    -- Adjust width to a multiple of 16px and at least 32px
    w = math.floor( w / 16 ) * 16
    if w < 32 then
        w = 32
    end
    print(w)
    Widget.__init( self, x, y, w, 36 )

    self.state = "normal"
    self.atlas = atlas
    self.nBitmaps = (w / 16)
    self.bitmaps = {}
    for i = 1, self.nBitmaps do
        self.bitmaps[i] = Bitmap()
        self.bitmaps[i].x = x + ( i * 16 ) - 16
        self.bitmaps[i].y = y
        rl:add( self.bitmaps[i] )
    end
    self.text = Text( caption, Button.font, Button.colors.normal, x + w / 2, y + 8 )
    rl:add( self.text )
    self.rl = rl

    self:updateVisualState()

end

function Button:destroy()
    for i = 1, self.nBitmaps do
        self.rl:remove( self.bitmaps[i] )
    end
    self.rl:remove( self.text )

    Widget:destroy(self)
end

function Button:move( x, y )
    if Widget.move( self, x, y ) then
        return true
    end

    for i = 1, self.nBitmaps do
        self.bitmaps[i].x = self.left
        self.bitmaps[i].y = self.top
    end
end

function Button:mouseClick( time, x, y, button )
    if self:intersects( x, y ) then
        if Widget.mouseClick( self, time, x, y, button ) then
            return true
        end
        if self.action then
            return self:action( time, x, y, button )
        end
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
        for i = 2, self.nBitmaps - 1 do
            self.bitmaps[i]:setBitmap( self.atlas:getFrame( 'btn_middle_' .. self.state ) )
        end
        self.bitmaps[self.nBitmaps]:setBitmap( self.atlas:getFrame( 'btn_right_' .. self.state ) )

        self.text.color = Button.colors[self.state]
        if self.oldstate == 'pressed' then
            self.text:setXY( self.left + self.width / 2, self.top + 8 )
        end

        if self.state == 'pressed' then
            self.text:setXY( self.left + self.width / 2 + 2, self.top + 10 )
        end

        self.oldstate = self.state
        return true -- state has changed
    end
end

function Button:lostMouse()
    self.state = "normal"
    self:updateVisualState()

    Widget.lostMouse(self)
end
