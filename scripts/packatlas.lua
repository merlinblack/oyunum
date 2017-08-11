require 'class'

class 'packatlas'

function packatlas:__init( directory, outputdir, name, width, height, scaling, padding )
    self.directory = directory .. '/'
    self.outputdir = outputdir .. '/'
    self.filename = name
    self.scaling = scaling
    self.padding = padding
    self.bitmaps = {}
    self.placed = {}
    self.names = {}
    self.composite = Bitmap()
    self.composite:create( width, height )
end

function packatlas:process()

    self:loadImages()

    self:sortImages()
    
    self:placeImages()
    
    self:saveComposite()
    
    self:writeFrameTable()

    self:compressFramwTable()

end

function packatlas:loadImages()
    for file in io.lines( self.directory .. 'imagelist.txt' ) do
        print( '^yellow^Loading:^!^', file )
        bitmap = Bitmap( self.directory .. file )
        self.names[bitmap] = file
        table.insert( self.bitmaps, bitmap )
    end
end

function packatlas:sortImages()
    table.sort( self.bitmaps, function( a, b ) return a.h > b.h end )
end

function packatlas:placeImages()
    local canvas = {}
    canvas.x = 0
    canvas.y = 0
    canvas.w = self.composite.w;
    canvas.h = self.composite.h;

    self:fillcanvas( canvas )
end

function packatlas:fillcanvas( canvas )

    if #self.bitmaps == 0 then return end -- no bitmaps left to place
    if canvas.w <= 0 then return end
    if canvas.h <= 0 then return end

    for index, bitmap in ipairs( self.bitmaps ) do
        if self:fits( canvas, bitmap ) then
            
            print( '^yellow^Placing image:^!^', self.names[bitmap] )
            self.composite:blit( bitmap, canvas.x, canvas.y, self.scaling )
            
            table.remove( self.bitmaps, index )

            bitmap.x = canvas.x
            bitmap.y = canvas.y
            table.insert( self.placed, bitmap )

            -- Horizontal
            local subcanvas = { x = canvas.x + bitmap.w + self.padding, y = canvas.y, w = canvas.w - bitmap.w - self.padding, h = bitmap.h + self.padding } 
            self:fillcanvas( subcanvas )
            -- Vertical
            local subcanvas = { x = canvas.x, y = canvas.y + bitmap.h + self.padding, w = canvas.w, h = canvas.h - bitmap.h - self.padding }
            self:fillcanvas( subcanvas )

            return
        end
    end
end

function packatlas:fits( canvas, image )
    if image.w + self.padding <= canvas.w and image.h + self.padding <= canvas.h then
        return true
    end
    return false
end

function packatlas:saveComposite()
    self.composite:saveBitmap( self.outputdir .. self.filename .. '.png' )
end
   
function packatlas:writeFrameTable()
    local frameTable = io.open( self.outputdir .. self.filename .. '.lua', 'w+' )
    frameTable:write( 'return {\n' )
    for _, bitmap in ipairs( self.placed ) do
        local name = self.names[bitmap]
        local desc = string.sub( name, 1, string.find( name, '.png' )-1 )
        desc = desc:gsub( '-', '_')
        desc = desc .. ' = { x = ' .. bitmap.x .. ', y = ' .. bitmap.y
        desc = desc .. ', w = ' .. bitmap.w .. ', h = ' .. bitmap.h .. ' },'
        frameTable:write( desc .. '\n' )
    end
    frameTable:write('}\n')
    frameTable:close()
end

function packatlas:compressFramwTable()
    local filename = self.outputdir .. self.filename .. '.lua'

    os.execute( 'rm -f ' .. filename .. '.gz' )
    os.execute( 'gzip ' .. filename )
end
