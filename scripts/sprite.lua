require 'class'
require 'dozipfile'

class 'Sprite' 

function Sprite:__init( file_base_name )
    self.bitmap = Bitmap( file_base_name .. '.png' )
    self.frames = dozipfile( file_base_name .. '.lua.gz' )
end

function Sprite:getFrame( name )
    if not self.frames then
        print( 'Could not get frame ' .. name .. '. No frame table.' )
    end

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
