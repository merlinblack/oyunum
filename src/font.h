#ifndef FONT_H
#define FONT_H

#include <allegro5/allegro_font.h>
#include <memory>

class Font
{
    ALLEGRO_FONT *mFont;

    public:

    Font( std::string filename, int size, int flags )
    {
        mFont = al_load_font( filename.c_str(), size, flags );
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

