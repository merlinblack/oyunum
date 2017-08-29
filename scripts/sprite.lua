require 'class'
require 'dozipfile'

class 'Sprite' 

function Sprite:__init( file_base_name )
    self.bitmaps = {}
    self.frames = {}
    self:addFrames( file_base_name )
end

function Sprite:addFrames( file_base_name )
    local newFrames = dozipfile( file_base_name .. '.lua.gz' )
    if newFrames == nil then
        print( 'Could not load frames from ' .. file_base_name .. '.lua.gz' )
        return
    end
    table.insert(self.bitmaps, Bitmap( file_base_name .. '.png' ))
    local bitmapIndex = #self.bitmaps
    for k, v in pairs( newFrames ) do
        v.bitmapIndex = bitmapIndex
        self.frames[k] = v
    end
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
            local atlas = self.bitmaps[f.bitmapIndex]
            b = atlas:getSubBitmap( f.x, f.y, f.w, f.h )
            f.bitmap = b
        end
    end

    return b
end

return Sprite
