#include "tilegrid.h"

void TileGrid::render()
{
    int x = dx;
    int y = dy;
    int column = 0;

    al_hold_bitmap_drawing( true );

    for( auto const tile : data ) {
        if( column >= width ) {
            column = 0;
            y += 32;
            x = dx;
        }
        if( tile != 0 ) {
            al_draw_bitmap(
                tileSet->getTile( tile ),
                x, y,
                0
            );
        }
        column++;
        x += 32;
    }

    al_hold_bitmap_drawing( false );
}

inline int TileGrid::getTile( int x, int y )
{
    return data[getOffset(x,y)];
}

inline void TileGrid::setTile( int x, int y, int tile )
{
    data[getOffset(x,y)] = tile;
}
