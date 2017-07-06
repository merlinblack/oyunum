#include "tileset.h"

void TileSet::destroy()
{
    for( auto const &tile : map ) {
        al_destroy_bitmap( tile.second );
    }
    map.clear();
    al_destroy_bitmap( source );
}

ALLEGRO_BITMAP* TileSet::createTileBitmap( int tileId )
{
    if( source == nullptr )
        return nullptr;

    int sourceWidth  = al_get_bitmap_width( source );

    int columns = sourceWidth / tileWidth;
    int row = tileId / columns;
    int col = tileId % columns;

    ALLEGRO_BITMAP* tile = al_create_sub_bitmap(
        source,
        col * tileWidth,
        row * tileHeight,
        tileWidth,
        tileHeight
    );

    map[tileId] = tile;

    return tile;
}

bool TileSet::loadSourceBitmap( std::string filename )
{
    if( source != nullptr ) {
        destroy();
    }

    source = al_load_bitmap( filename.c_str() );

    if( source == nullptr )
        return false;

    return true;
}

ALLEGRO_BITMAP* TileSet::getTile( int tileId )
{
    tileId -= firstTileId;

    TileMap::const_iterator entry = map.find( tileId );

    if( entry != map.end() )
    {
        return entry->second;
    }

    return createTileBitmap( tileId );
}
