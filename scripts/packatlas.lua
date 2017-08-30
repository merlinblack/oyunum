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
    self.composite = Bitmap()
    self.composite:create( width, height )
    self.width = width
    self.height = height
end

function packatlas:process()

    self:loadImages()

    coroutine.yield()

    self:sortImages()
    
    coroutine.yield()

    self.atlasIndex = 1
    self.composite.name = self:getIndexedFilename()

    local allPlaced = false

    while allPlaced == false do

        self:placeImages()
    
        coroutine.yield()

        self:saveComposite()
    
        coroutine.yield()


        if #self.bitmaps == 0 then
            allPlaced = true
        else
            self.composite = Bitmap()
            self.composite:create( self.width, self.height )
            self.atlasIndex = self.atlasIndex + 1
            self.composite.name = self:getIndexedFilename()
        end
    end

    self:writeFrameTable()

    coroutine.yield()

    self:compressFrameTable()

    print( 'Completed packing atlas for ' .. self.filename  .. '.' )

end

function packatlas:loadImages()
    for file in io.lines( self.directory .. 'imagelist.txt' ) do
        print( '^yellow^Loading:^!^', file )
        bitmap = Bitmap( self.directory .. file )
        bitmap.name = file
        bitmap.sw = bitmap.w * self.scaling
        bitmap.sh = bitmap.h * self.scaling
        table.insert( self.bitmaps, bitmap )
        coroutine.yield()
    end
end

function packatlas:sortImages()
    function imageSort( a, b )
        if a.h == b.h then
            return a.name < b.name
        end
        return a.h > b.h
    end
    table.sort( self.bitmaps, imageSort )
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
            
            print( '^yellow^Placing image:^!^', bitmap.name )
            print( bitmap, canvas.x, canvas.y, self.scaling )
            self.composite:blit( bitmap, canvas.x, canvas.y, self.scaling )
            
            table.remove( self.bitmaps, index )

            bitmap.x = canvas.x
            bitmap.y = canvas.y
            bitmap.compositeName = self.composite.name
            table.insert( self.placed, bitmap )

            -- Horizontal
            local subcanvas = { x = canvas.x + bitmap.sw + self.padding, y = canvas.y, w = canvas.w - bitmap.sw - self.padding, h = bitmap.sh + self.padding }
            self:fillcanvas( subcanvas )
            -- Vertical
            local subcanvas = { x = canvas.x, y = canvas.y + bitmap.sh + self.padding, w = canvas.w, h = canvas.h - bitmap.sh - self.padding }
            self:fillcanvas( subcanvas )

            coroutine.yield()

            return
        end
    end
end

function packatlas:fits( canvas, image )
    if image.sw + self.padding <= canvas.w and image.sh + self.padding <= canvas.h then
        return true
    end
    return false
end

function packatlas:getIndexedFilename()
    local filename = self.filename
    if self.atlasIndex > 1 then
        filename = filename .. self.atlasIndex
    end
    return filename
end

function packatlas:saveComposite()
    local filename = self.composite.name
    filename = self.outputdir .. filename .. '.png'

    print( 'Saving composite image ' .. filename  )
    if not self.composite:saveBitmap( filename ) then
        print( 'Could not save image.' )
    end
end
   
function packatlas:writeFrameTable()
    local filename = self.filename
    filename = self.outputdir .. filename .. '.lua'
    local frameTable = io.open( filename, 'w+' )
    frameTable:write( 'return {\n' )
    for _, bitmap in ipairs( self.placed ) do
        coroutine.yield()
        local name = bitmap.name
        local desc = string.sub( name, 1, string.find( name, '.png' )-1 )
        print( 'Writing frame table entry for ' .. name )
        desc = desc:gsub( '-', '_')
        desc = desc .. ' = { src = \'' .. bitmap.compositeName .. '\''
        desc = desc .. ', x = ' .. bitmap.x .. ', y = ' .. bitmap.y
        desc = desc .. ', w = ' .. bitmap.sw .. ', h = ' .. bitmap.sh .. ' },'
        frameTable:write( desc .. '\n' )
    end
    frameTable:write('}\n')
    frameTable:close()
end

function packatlas:compressFrameTable()
    local filename = self.filename
    filename = self.outputdir .. filename .. '.lua'

    print( 'Gzipping ' .. filename )
    os.remove( filename .. '.gz' )
    os.execute( 'gzip ' .. filename .. ' &')
end
