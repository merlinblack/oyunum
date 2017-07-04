#ifndef BITMAP_H
#define BITMAP_H

#include <allegro5/allegro.h>
#include <memory>
#include "renderlist.h"

// This class does double duty as a renderable, and a wrapper for allegro bitmap.
class Bitmap;

using BitmapPtr = std::shared_ptr<Bitmap>;

class Bitmap : public Renderable
{
    ALLEGRO_BITMAP* bitmap;
    int x, y;

    public:

    Bitmap() : bitmap(nullptr), x(0), y(0)
    {}
    Bitmap( ALLEGRO_BITMAP* _bitmap, int _x, int _y  ) : bitmap(_bitmap), x(_x), y(_y)
    {}

    void render()
    {
        al_draw_bitmap( bitmap, x, y, 0 );
    }

    bool loadFromFile( std::string filename )
    {
        bitmap = al_load_bitmap( filename.c_str() );

        return bitmap != nullptr;
    }
    
    BitmapPtr getSubBitmap( int _x, int _y, int _w, int _h )
    {
        ALLEGRO_BITMAP *sub = al_create_sub_bitmap( bitmap, _x, _y, _w, _h );

        return std::make_shared<Bitmap>( sub, 0, 0 );
    }

    int getX() { return x; }
    int getY() { return y; }
    
    void setX( int _x ) { x = _x; }
    void setY( int _y ) { y = _y; }

};

#endif // BITMAP_H
