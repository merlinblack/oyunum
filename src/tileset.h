#ifndef TILESET_H
#define TILESET_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>
#include <map>

using TileMap = std::map<int,ALLEGRO_BITMAP*>;

class TileSet
{
    ALLEGRO_BITMAP* source;
    
    int firstTileId;

    TileMap map;

    int tileWidth;
    int tileHeight;

    ALLEGRO_BITMAP* createTileBitmap( int tileId );

    void destroy();

    public:
    TileSet( int first = 1, int tWidth = 32, int tHeight = 32 ) 
        : source(nullptr)
        , firstTileId(first)
        , tileWidth(tWidth)
        , tileHeight(tHeight)
    {}

    ~TileSet()
    {
        destroy();
    }

    bool loadSourceBitmap( std::string filename );
    ALLEGRO_BITMAP* getTile( int tileId );

    int getFirstTileID() { return firstTileId; }
};

using TileSetPtr = std::shared_ptr<TileSet>;

#endif // TILESET_H
