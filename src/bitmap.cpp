#include "bitmap.h"
#include <string>

Bitmap::~Bitmap()
{
    al_destroy_bitmap( bitmap );
    bitmap = nullptr;
}

void Bitmap::render()
{
    if( bitmap != nullptr )
    {
        int w = al_get_bitmap_width( bitmap );
        int h = al_get_bitmap_height( bitmap );
        al_draw_scaled_bitmap( bitmap, 0, 0, w, h, x, y, scale * w, scale * h, 0 );
    }
}

// Creates a blank transparent bitmap.
void Bitmap::create( int w, int h, int flags )
{
    int old_flags = al_get_new_bitmap_flags();
    al_set_new_bitmap_flags( old_flags | flags );

    bitmap = al_create_bitmap( w, h );
    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap( bitmap );
    al_clear_to_color( al_map_rgba( 0, 0, 0, 0 ) );
    al_set_target_bitmap( prev );
    al_set_new_bitmap_flags( old_flags );
}

void Bitmap::blit( const BitmapPtr& other, float x, float y, float scale )
{
    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    al_set_target_bitmap( bitmap );

    int w = al_get_bitmap_width( other->bitmap );
    int h = al_get_bitmap_height( other->bitmap );

    al_draw_scaled_bitmap( other->bitmap,
        0, 0,
        w, h,
        x, y,
        w * scale, h * scale,
        0 );

    al_set_target_bitmap( prev );
}

bool Bitmap::loadFromFile( const std::string& filename, int flags )
{
    int old_flags = al_get_new_bitmap_flags();

    al_set_new_bitmap_flags( old_flags | flags );

    bitmap = al_load_bitmap( filename.c_str() );

    al_set_new_bitmap_flags( flags );

    return bitmap != nullptr;
}

bool Bitmap::saveToFile( const std::string& filename )
{
    return al_save_bitmap( filename.c_str(), bitmap );
}

BitmapPtr Bitmap::getSubBitmap( int _x, int _y, int _w, int _h )
{
    ALLEGRO_BITMAP *sub = al_create_sub_bitmap( bitmap, _x, _y, _w, _h );

    return std::make_shared<Bitmap>( sub, 0, 0 );
}

void Bitmap::setBitmap( ALLEGRO_BITMAP* nb )
{
    bitmap = nb;
}

void Bitmap::setBitmap( BitmapPtr& nb )
{
    // Copy but don't clone.
    // I.e. get a new pointer nb's bitmap 
    // that can be free'd with out causing
    // nb to seg fault when destructing.
    // We don't want to set nb's pointer to
    // nullptr either.
    al_destroy_bitmap( bitmap );
    bitmap = al_create_sub_bitmap( nb->bitmap,
           0, 0,
           al_get_bitmap_width( nb->bitmap ),
           al_get_bitmap_height( nb->bitmap )
           );
}

