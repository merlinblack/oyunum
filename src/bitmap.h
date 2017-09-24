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
    float x, y;
    float scale;

    public:

    Bitmap() : bitmap(nullptr), x(0), y(0), scale(1.0f)
    {}
    Bitmap( ALLEGRO_BITMAP* _bitmap, int _x = 0 , int _y = 0 ) : bitmap(_bitmap), x(_x), y(_y), scale(1.0f)
    {}

    ~Bitmap();

    void render();
    void create( int w, int h, int flags = 0 );
    bool loadFromFile( const std::string& filename, int flags = 0 );
    bool saveToFile( const std::string& filename );
    BitmapPtr getSubBitmap( int _x, int _y, int _w, int _h );
    void setBitmap( ALLEGRO_BITMAP* nb );
    void setBitmap( BitmapPtr& nb );
    void blit( const BitmapPtr& other, float x, float y, float scale );

    float getX() { return x; }
    float getY() { return y; }

    float getScale() { return scale; }
    
    void setX( int _x ) { x = _x; }
    void setY( int _y ) { y = _y; }
    
    int getWidth() { return al_get_bitmap_width( bitmap ); }
    int getHeight() { return al_get_bitmap_height( bitmap ); }

    void setScale( float _scale ) { scale = _scale; }

};

#endif // BITMAP_H
