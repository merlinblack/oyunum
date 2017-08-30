require 'class'
require 'dozipfile'

class 'Sprite' 

function Sprite:__init( dir, name )
    self.bitmaps = {}
    self.frames = {}
    self.dir = dir .. '/'
    self:addFrames( dir, name )
end

function Sprite:addFrames( dir, name )
    local filename = dir .. '/' .. name .. '.lua.gz'
    local newFrames = dozipfile( filename )
    if newFrames == nil then
        print( 'Could not load frames from ' .. filename .. '.lua.gz' )
        return
    end
    for k, v in pairs( newFrames ) do
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
            local atlas = self.bitmaps[f.src]
            if not atlas then
                atlas = Bitmap( self.dir .. f.src .. '.png' )
                if not atlas then
                    print( 'Could not load image ' .. self.dir .. f.src .. '.png' )
                    return nil
                end
                self.bitmaps[f.src] = atlas
            end
            b = atlas:getSubBitmap( f.x, f.y, f.w, f.h )
            f.bitmap = b
        end
    end

    return b
end

return Sprite
