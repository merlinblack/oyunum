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

    void render();
    void create( int w, int h );
    bool loadFromFile( const std::string& filename );
    bool saveToFile( const std::string& filename );
    BitmapPtr getSubBitmap( int _x, int _y, int _w, int _h );
    void setBitmap( ALLEGRO_BITMAP* nb );
    void setBitmap( BitmapPtr& nb );
    void blit( const BitmapPtr& other, int x, int y, float scale );

    int getX() { return x; }
    int getY() { return y; }

    float getScale() { return scale; }
    
    void setX( int _x ) { x = _x; }
    void setY( int _y ) { y = _y; }

    void setScale( float _scale ) { scale = _scale; }

};

#endif // BITMAP_H
