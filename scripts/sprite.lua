require 'class'

class 'Sprite' 

function Sprite:__init( file_base_name )
    self.bitmap = Bitmap( file_base_name .. '.png' )
    self.frames = dofile( file_base_name .. '.lua' )
end

function Sprite:getframe( name )
    local f = self.frames[name]
    local b

    if f then
        b = f.bitmap 
        if not b then
            b = self.bitmap:getSubBitmap( f.x, f.y, f.w, f.h )
            f.bitmap = b
        end
    end

    return b
end

return Sprite
