#ifndef TILEGRID_H
#define TILEGRID_H

#include "renderlist.h"
#include "tileset.h"
#include <vector>
#include <memory>

using TileGridData = std::vector<int>;

class TileGrid : public Renderable
{
    TileSetPtr tileSet;
    int width;
    int dx;
    int dy;

    TileGridData data;

    inline int getOffset( int x, int y )
    {
        return (y * width) + x;
    }

    public:

    TileGrid( TileSetPtr& ts, int w ) 
        : tileSet(ts)
        , width(w)
        , dx(0)
        , dy(0)
    {}

    void setData( TileGridData newData )
    {
        data.swap( newData );
    }

    void setXY( int x, int y )
    {
        dx = x, dy = y;
    }

    void render();

    inline int getTile( int x, int y );
    inline void setTile( int x, int y, int tile );
};

using TileGridPtr = std::shared_ptr<TileGrid>;

#endif // TILEGRID_H
