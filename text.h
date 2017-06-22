#ifndef TEXT_H
#define TEXT_H

#include <allegro5/allegro_font.h>
#include <sstream>
#include <memory>
#include "renderlist.h"

class Text : public Renderable
{
    std::string mText;
    ALLEGRO_FONT *mFont;
    ALLEGRO_COLOR mColor;
    int mX;
    int mY;

    public:

    Text( ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y )
        : mFont(font), mColor(color),  mX(x), mY(y)
    {}

    Text( std::string&& text, ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y )
        : mText(std::move(text)), mFont(font), mColor(color),  mX(x), mY(y)
    {}

    void setText( std::string&& text )
    {
        mText = std::move(text);
    }

    std::string& getText()
    {
        return mText;
    }
    
    void clearText()
    {
        mText.clear();
    }

    void render()
    {
        al_draw_text( mFont, mColor, mX, mY, ALLEGRO_ALIGN_CENTRE, mText.c_str() );
    }

};

using TextPtr = std::shared_ptr<Text>;

template<typename T>
TextPtr operator<<( TextPtr tp, T t )
{
    std::ostringstream stream( tp->getText(), std::ostringstream::ate );

    stream << t;

    tp->setText( stream.str() );

    return tp;
}


#endif // TEXT_H

