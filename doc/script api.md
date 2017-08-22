Oyunum Script C++ Bindings
==========================

Index
-------
Bitmap

Color

Font

Renderable

Render List

Text

Tile Grid

Tile Set

Console

Misc

[C] denotes a constructor.
***
Bitmap (Renderable)
---------------------
#### Properties:
x	-	x co-ordinate of upper left corner for rendering.

y	-	y co-ordinate of upper left corner for rednering.

w	-	width in pixels.

h	-	height in pixels.

scale	-	scale for rendering.

#### Methods:
[C] Bitmap( [filename] )	-	Construct a bitmap and optionally load an image file.

blit( Bitmap, x, y, scale )	-	Draw the given bitmap at x, y and scaled to scale on the bitmap.

create( width, height )	-	Create a bitmap of given diamensions and filled with transparent color.

loadfile( filename )	-	Load bitmap image data from a file.

getSubBitmap( x, y, w, h )	-	Grab a sub-bitmap from the given co-ordinates and width x height.

saveBitmap( filename )	-	Save the bitmap image data to filename. Format is deduced from the file extension.

setBitmap( Bitmap )	-	Replace (if there is one) the current bitmap with the bitmap from Bitmap.

***
Color
-------
#### Properties:
r	-	red component of the color (0-255)

g	-	green component of the color (0-255)

b	-	blue component of the color (0-255)

a	-	alpha component of the color (0-255)

#### Methods:
[C] Color( r, g, b, a )	-	Construct a color object with the given color values.
***
Font
------
#### Properties:
None.
#### Methods:
[C] Font( filename, size )	-	Construct a font and load the font from filename at the given size.
***
Renderable
------------
#### Properties:
None.
#### Methods:
None.
***
Render List (Renderable)
--------------------------
#### Properties:
None.
#### Methods:
[C] Renderable()	-	create a new render list.

sort()	-	re-sort list by renderable sort order.

insert(r)	-	insert renderable r in the list according to it's sort order.

add(r)	-	add renderable to end of the list and adjust it's sort order.

remove(r)	-	remove renderable from the list.
***
Text (Renderable)
-------------------
#### Properties:
color	-	The color of the text.
text	-	The text.
#### Methods:
[C] Text( text, font, color, x, y )	-	Create a text renderable with the given parameters.

setXY( x, y )	-	Change the x, y co-ordinates for rendering.
***
Tile Grid (Renderable)
------------------------
#### Properties:
None.
####Methods:
[C] TileGrid( tileset, width, height )	-	Create a new Tile Grid using the tileset and width and height in tiles.

setData( data )	-	Load tile data from the given Lua table of integer tile indexes.

setXY( x, y )	-	Set x, and y co-ordinates for rendering.
***
Tile Set
----------
#### Properties:
None.
#### Methods:
[C] TileSet( [first], [w], [h] )	-	Create a tile set with the igven optional parameters, fist - the first tile id of this set, w and h - the width and height of each tile.

loadSourceBitmap( filename )	-	Load tile set image file. Returns true on success.

getTile( id )	-	Get a sub-bitmap coresponding to the given tile id.
***
Console
---------
#### Properties:
None.
#### Free functions:
print( ... )	-	Converts each argument to a string then displays in the console separated by a tab character. Color can be specified in text between '^' (hat) characters. For example "Hello ^red^world.". The sequence ^!^ returns to 'normal' color.

console( command_string )	-	Sends command_string to the console. Commands are: 'open', 'close', 'toggle', and 'clear'.
***
Miscellaneous
---------------
#### Free Functions:
quit()	-	Cuases the render loop to exit and exit the program.
