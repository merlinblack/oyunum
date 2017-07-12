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
    float scale;
    int w, h;

    public:

    Bitmap() : bitmap(nullptr), x(0), y(0), scale(1.0f)
    {}
    Bitmap( ALLEGRO_BITMAP* _bitmap, int _x = 0 , int _y = 0 ) : bitmap(_bitmap), x(_x), y(_y), scale(1.0f)
    {}

    void render()
    {
        if( bitmap != nullptr )
        {
            w = al_get_bitmap_width( bitmap );
            h = al_get_bitmap_height( bitmap );
            al_draw_scaled_bitmap( bitmap, 0, 0, w, h, x, y, scale * w, scale * h, 0 );
        }
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

    void setBitmap( ALLEGRO_BITMAP* nb )
    {
        bitmap = nb;
    }

    void setBitmap( BitmapPtr& nb )
    {
        bitmap = nb->bitmap;
    }

    int getX() { return x; }
    int getY() { return y; }

    float getScale() { return scale; }
    
    void setX( int _x ) { x = _x; }
    void setY( int _y ) { y = _y; }

    void setScale( float _scale ) { scale = _scale; }

};

#endif // BITMAP_H
