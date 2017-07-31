#ifndef TEXT_H
#define TEXT_H

#include <sstream>
#include <memory>
#include "renderlist.h"
#include "font.h"

class Text : public Renderable
{
    std::string mText;
    FontPtr mFont;
    ALLEGRO_COLOR mColor;
    int mX;
    int mY;

    public:

    Text( FontPtr font, ALLEGRO_COLOR color, int x, int y )
        : mFont(font), mColor(color),  mX(x), mY(y)
    {}

    Text( const std::string&& text, FontPtr font, ALLEGRO_COLOR color, int x, int y )
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

    void setColor( ALLEGRO_COLOR color )
    {
        mColor = color;
    }

    void setXY( int x, int y )
    {
        mX = x;
        mY = y;
    }

    ALLEGRO_COLOR getColor()
    {
        return mColor;
    }

    void clearText()
    {
        mText.clear();
    }

    void render()
    {
        al_draw_text( mFont->get(), mColor, mX, mY, ALLEGRO_ALIGN_CENTRE, mText.c_str() );
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

