require 'gui/widget'

class 'DragButton' (Widget)

function DragButton:__init( rl, x, y )

    Widget.__init( self, x, y, 32, 32 )

    self.state = "normal"
    self.oldstate = "normal"
    self.bitmap = Bitmap( 'data/icon.tga' )
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

    self.bitmap.x = self.left
    self.bitmap.y = self.top
end

function DragButton:mouseMoved( x, y, button )
    if self:intersects( x, y ) then
        if button == 0 and self.state == "click" then -- click release
            self.state = "hover"
        end
        if self.state == "normal" then
            self.state = "hover"
        end
        if self.state == "click" then
            self:move( x, y )
        end
    else
        self.state = "normal"
    end
    if self.state == "hover" and button == 1 then
        self.state = "click"
    end

    self:updateVisualState()
end

function DragButton:updateVisualState()
    if self.state ~= self.oldstate then
        if self.state == "normal" then
            print( 'NORMAL' )
        elseif self.state == "hover" then
            print( 'HOVER' )
        else
            print( 'PRESSED' )
        end
        self.oldstate = self.state
    end
end

function DragButton:lostMouse()
    self.state = "normal"
    self:updateVisualState()

    for _,child in pairs( self.children ) do
        if child.lostMouse then child:lostMouse() end
    end
end
