#ifndef FONT_H
#define FONT_H

#include <allegro5/allegro_font.h>
#include <memory>
#include <string>

class Font
{
    ALLEGRO_FONT *mFont;

    public:

    Font( std::string filename, int size, int flags )
    {
        int originalFlags = al_get_new_bitmap_flags();
        al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
        mFont = al_load_font( filename.c_str(), size, flags );
        al_set_new_bitmap_flags( originalFlags );
    }

    ~Font()
    {
        al_destroy_font( mFont );
    }

    ALLEGRO_FONT* get()
    {
        return mFont;
    }
};

using FontPtr = std::shared_ptr<Font>;

#endif // FONT_H

