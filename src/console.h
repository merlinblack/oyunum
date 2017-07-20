#ifndef CONSOLE_H
#define CONSOLE_H

#include <lua.hpp>
#include <string>
#include <list>
#include <memory>
#include "renderlist.h"
#include "font.h"
#include "editstring.h"

#define CONSOLE_MAX_LINES 128
#define CONSOLE_LINE_LENGTH 79
#define CONSOLE_TAB_STOP 8
#define CONSOLE_LINE_COUNT 15

#include <iostream>
using std::cout;
using std::endl;

using StringVector = std::list<std::string>;

class Console : public Renderable
{
    StringVector mLines;
    EditString command;
    std::string prompt;
    FontPtr mFont;
    lua_State* mL;
    int width;
    bool visible;

    int mStartLine;
    double cursorBlinkTime;
    bool cursorBlink;

    public:

    Console( lua_State* L, FontPtr font, int _width )
        : mFont(font), mL(L), width(_width), visible(true), mStartLine(0), cursorBlink(0)
    {
        lua_pushlightuserdata( mL, this );
        lua_setfield( mL, LUA_REGISTRYINDEX, "console" );
        lua_getglobal( mL, "print" );
        lua_setglobal( mL, "oldprint" );
        lua_register( mL, "print", luaprint );
        prompt = "Lua> ";
        command.setText( "print( 'Çilek seviyorum' )" );
    }

    ~Console()
    {
        lua_pushnil( mL );
        lua_setfield( mL, LUA_REGISTRYINDEX, "console" );
        lua_getglobal( mL, "oldprint" );
        lua_setglobal( mL, "print" );
    }

    bool isVisible()
    {
        return visible;
    }

    void toggleVisibility()
    {
        visible = ! visible;
    }

    void clear()
    {
        mLines.clear();
    }

    bool injectKeyPress( const ALLEGRO_EVENT& event )
    {
        if( ! visible )
            return false;

        if( event.keyboard.keycode == ALLEGRO_KEY_PGUP )
        {
            pageUp();
            return true;
        }

        if( event.keyboard.keycode == ALLEGRO_KEY_PGDN )
        {
            pageDown();
            return true;
        }

        return command.injectKeyPress( event );
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
                column = 1;
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
            mStartLine = std::max( (int)mLines.size() - (CONSOLE_LINE_COUNT - 1), 0 );

        return;
    }

    static int luaprint( lua_State* L )
    {
        std::string output;
        int nArgs = lua_gettop( L );
        lua_getfield( L, LUA_REGISTRYINDEX, "console" );
        Console *self = static_cast<Console*>(lua_touserdata( L, -1 ));

        lua_getglobal( L, "tostring" );

        for( int index = 1; index <= nArgs; index++ )
        {
            const char *s;

            lua_pushvalue( L, -1 ); // tostring func
            lua_pushvalue( L, index );
            lua_pcall( L, 1, 1, 0 );
            s = lua_tostring( L, -1 );

            if( !s ) {
                return luaL_error( L, "'tostring' must return a string to 'print'" );
            }

            output += s;

            if( index < nArgs )
                output += "\t";

            lua_pop( L, 1 );
        }

        self->print( output );
    }

    void render()
    {
        if( !visible )
            return;

        int y = 0;
        int line_no = 1;
        ALLEGRO_COLOR color = al_map_rgb( 255, 255, 255 );
        ALLEGRO_COLOR curcolor = al_map_rgb( 255, 50, 50 );

        al_draw_filled_rectangle( 0, 0, width, CONSOLE_LINE_COUNT*16, al_map_rgba( 0, 0, 0, 200 ) );

        for( const auto& line : mLines )
        {
            if( line_no >= mStartLine && y < (CONSOLE_LINE_COUNT-1)*16 )
            {
                al_draw_text( mFont->get(), color, 5, y, ALLEGRO_ALIGN_LEFT, line.c_str() );
                y += 16;
            }
            line_no++;
        }

        std::stringstream ss;

        ss << prompt;
        ss << command.getText();

        al_draw_text( mFont->get(), color, 5, y, ALLEGRO_ALIGN_LEFT, ss.str().c_str() );

        double t = al_get_time();
        if( t - cursorBlinkTime > 0.7 )
        {
            cursorBlinkTime = t;
            cursorBlink = ! cursorBlink;
        }

        if( cursorBlink )
        {
            int charwidth = al_get_text_width( mFont->get(), "X" );
            int x = charwidth * ( command.getPosition() + prompt.length());
            int ys = y + 1;
            if( !command.isOverwriting() ) {
                ys += 14;
            }
            al_draw_rectangle( x+5, ys, x+13, y+15, curcolor, 1 );
        }
    }

    void pageUp()
    {
        mStartLine = std::max( 0, mStartLine - 5 );
    }

    void pageDown()
    {
        mStartLine = std::min( (int)mLines.size() - CONSOLE_LINE_COUNT + 1, mStartLine + 5 );
    }

};

using ConsolePtr = std::shared_ptr<Console>;

#endif // CONSOLE_H

