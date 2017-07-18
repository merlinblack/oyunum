#ifndef CONSOLE_H
#define CONSOLE_H

#include <allegro5/allegro_font.h>
#include <string>
#include <list>
#include <memory>
#include "renderlist.h"

#define CONSOLE_MAX_LINES 128
#define CONSOLE_LINE_LENGTH 160
#define CONSOLE_TAB_STOP 8
#define CONSOLE_LINE_COUNT 25

#include <iostream>
using std::cout;
using std::endl;

using StringVector = std::list<std::string>;

class Console : public Renderable
{
    StringVector mLines;
    ALLEGRO_FONT *mFont;
    ALLEGRO_COLOR mColor;
    int mX;
    int mY;
    int mW;
    int mH;
    int mStartLine;

    public:

    Console( ALLEGRO_FONT* font, ALLEGRO_COLOR color, int x, int y, int w, int h )
        : mFont(font), mColor(color), mX(x), mY(y), mW(w), mH(h), mStartLine(0)
    {}

    void setColor( ALLEGRO_COLOR color )
    {
        mColor = color;
    }

    ALLEGRO_COLOR getColor()
    {
        return mColor;
    }

    void clear()
    {
        mLines.clear();
    }

    void addLine( const std::string& line )
    {
        if( mLines.size() > CONSOLE_MAX_LINES-1 )
            mLines.pop_front();

        mLines.push_back( line );

        cout << line << endl;
    }

    void print( const std::string& text )
    {
        std::string line;
        char buffer[5];
        int pos = 0;
        int column;

        ALLEGRO_USTR* unicode = al_ustr_new( text.c_str() );
        int32_t codepoint;

        column = 1;

        while( (codepoint = al_ustr_get_next( unicode, &pos )) != -1 )
        {

            if( codepoint == '\n' || column > CONSOLE_LINE_LENGTH )
            {
                addLine( line );
                line.clear();
                if( codepoint != '\n' ) {
                    al_ustr_prev( unicode, &pos );      //we want the current char for the next line
                }
                column = 0;
            }
            else if( codepoint == '\t' )
            {
                // Push at least one space.
                line.push_back(' ');
                column++;

                // fill until next multiple of CONSOLE_TAB_STOP
                while(( column % CONSOLE_TAB_STOP ) != 0 )
                {
                    line.push_back(' ');
                    column++;
                }
            }
            else
            {
                size_t writen = al_utf8_encode( buffer, codepoint );
                buffer[writen] = 0;
                line.append( buffer );
                column++;
            }
        }
        if( line.length() )
        {
            addLine( line );
        }

        // Make sure last text printed is in view.
        if( mLines.size() > CONSOLE_LINE_COUNT - 1 )
            mStartLine = mLines.size() - (CONSOLE_LINE_COUNT - 1);

        return;
    }

    void render()
    {
        //al_draw_text( mFont, mColor, mX, mY, ALLEGRO_ALIGN_CENTRE, mText.c_str() );
    }

};

using ConsolePtr = std::shared_ptr<Console>;

#endif // CONSOLE_H

