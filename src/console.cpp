#include "config.h"
#include "console.h"
#include <allegro5/allegro_primitives.h>
#include "LuaRef.h"
#include "colortext.h"

#define CONSOLE_MAX_LINES 128
#define CONSOLE_LINE_LENGTH 79
#define CONSOLE_TAB_STOP 8
#define CONSOLE_TEXT_HEIGHT 16
#define CONSOLE_LINE_COUNT (int)(SCREEN_H / CONSOLE_TEXT_HEIGHT * 0.75)
#define CONSOLE_MAX_HISTORY 128
#define LUA_CONSOLE_INDEX "console"

#include <iostream>
using std::cout;
using std::endl;

Console::Console( lua_State* L, FontPtr font, int _width )
        : mFont(font), mL(L), width(_width), visible(true), mStartLine(0), cursorBlink(0)
{
    lua_pushlightuserdata( mL, this );
    lua_setfield( mL, LUA_REGISTRYINDEX, LUA_CONSOLE_INDEX );
    lua_getglobal( mL, "print" );
    lua_setglobal( mL, "oldprint" );
    lua_register( mL, "print", luaPrint );
    lua_register( mL, "console", luaCommand );
    interpreter.init( mL );
}

Console::~Console()
{
    lua_pushnil( mL );
    lua_setfield( mL, LUA_REGISTRYINDEX, LUA_CONSOLE_INDEX );
    lua_getglobal( mL, "oldprint" );
    lua_setglobal( mL, "print" );
    lua_pushnil( mL );
    lua_setglobal( mL, "console" );
}

bool Console::isVisible()
{
    return visible;
}

void Console::toggleVisibility()
{
    visible = ! visible;
}

void Console::clear()
{
    mLines.clear();
    mStartLine = 1;
}

int Console::luaCommand( lua_State* L )
{
    static const char* opts[] = { "open", "close", "toggle", "clear", "savehistory", nullptr };

    lua_getfield( L, LUA_REGISTRYINDEX, LUA_CONSOLE_INDEX );
    Console *self = static_cast<Console*>(lua_touserdata( L, -1 ));

    int cmd = luaL_checkoption( L, 1, nullptr, opts );

    switch( cmd )
    {
        case 0:
            self->visible = true;
            break;
        case 1:
            self->visible = false;
            break;
        case 2:
            self->visible = ! self->visible;
            break;
        case 3:
            self->clear();
            break;
        case 5:
            // savehistory
            break;
    }

    return 0;
}

bool Console::injectKeyPress( const ALLEGRO_EVENT& event )
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

    if( event.keyboard.keycode == ALLEGRO_KEY_ENTER || 
            event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER )
    {
        execute();
        return true;
    }

    if( event.keyboard.keycode == ALLEGRO_KEY_UP )
    {
        historyUp();
        return true;
    }

    if( event.keyboard.keycode == ALLEGRO_KEY_DOWN )
    {
        historyDown();
        return true;
    }

    if( event.keyboard.keycode == ALLEGRO_KEY_TAB )
    {
        autoComplete();
        return true;
    }

    return command.injectKeyPress( event );
}

void Console::resume()
{
    interpreter.resume();
}

void Console::addLine( const std::string& line )
{
    if( mLines.size() > CONSOLE_MAX_LINES-1 )
        mLines.pop_front();

    mLines.push_back( line );

    cout << strip_color_text( line ) << endl;
}

void Console::print( const std::string& text )
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
        mStartLine = std::max( (int)mLines.size() - CONSOLE_LINE_COUNT + 2, 0 );

    al_ustr_free( unicode );

    return;
}

int Console::luaPrint( lua_State* L )
{
    std::string output;
    int nArgs = lua_gettop( L );
    lua_getfield( L, LUA_REGISTRYINDEX, LUA_CONSOLE_INDEX );
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

    return 0;
}

void Console::render()
{
    if( !visible )
        return;

    int y = 0;
    int line_no = 1;
    ALLEGRO_COLOR color = al_map_rgb( 255, 255, 255 );
    ALLEGRO_COLOR curcolor = al_map_rgb( 255, 50, 50 );

    al_draw_filled_rectangle( 0, 0, width, CONSOLE_LINE_COUNT*CONSOLE_TEXT_HEIGHT, al_map_rgba( 0, 0, 0, 200 ) );

    for( const auto& line : mLines )
    {
        if( line_no >= mStartLine && y < (CONSOLE_LINE_COUNT-1)*16 )
        {
            draw_colored_text( mFont->get(), color, 5, y, ALLEGRO_ALIGN_LEFT, line.c_str() );
            y += CONSOLE_TEXT_HEIGHT;
        }
        line_no++;
    }

    std::string prompt = interpreter.getPrompt();

    int prompt_width = draw_colored_text( mFont->get(), color, 5, y, ALLEGRO_ALIGN_LEFT, prompt.c_str() );

    al_draw_text( mFont->get(), color, 5 + prompt_width, y, ALLEGRO_ALIGN_LEFT, command.getText().c_str() );

    double t = al_get_time();
    if( t - cursorBlinkTime > 0.7 )
    {
        cursorBlinkTime = t;
        cursorBlink = ! cursorBlink;
    }

    if( cursorBlink )
    {
        int charwidth = al_get_text_width( mFont->get(), "X" );
        int x = charwidth * command.getPosition() + prompt_width;
        int ys = y + 1;
        if( !command.isOverwriting() ) {
            ys += 14;
        }
        al_draw_rectangle( x+5, ys, x+13, y+15, curcolor, 1 );
    }
}

void Console::pageUp()
{
    mStartLine = std::max( 0, mStartLine - 5 );
}

void Console::pageDown()
{
    mStartLine = std::min( (int)mLines.size() - CONSOLE_LINE_COUNT + 2, mStartLine + 5 );
}

void Console::execute()
{
    print( interpreter.getPrompt() + command.getText() );
    interpreter.insertLine( command.getText() );
    historyAdd( command.getText() );
    command.clear();
    autoCompleteClear();
}

void Console::historyUp()
{
    if( ! mHistory.empty() )
    {
        if( mHistoryLine == mHistory.begin() )
        {
            mHistoryLine = mHistory.end();
        }
        mHistoryLine--;
        command.setText( *mHistoryLine );
    }
}

void Console::historyDown()
{
    if( ! mHistory.empty() )
    {
        mHistoryLine++;
        if( mHistoryLine == mHistory.end() )
        {
            mHistoryLine = mHistory.begin();
        }
        command.setText( *mHistoryLine );
    }
}

void Console::historyAdd( const std::string& cmd )
{
    mHistory.remove( cmd );
    mHistory.push_back( cmd );
    if( mHistory.size() > CONSOLE_MAX_HISTORY )
    {
        mHistory.pop_front();
    }
    mHistoryLine = mHistory.end();
}

void Console::autoComplete()
{
    ManualBind::LuaRef complete( mL, "autoComplete" );
    if( complete.isFunction() )
    {
        try {
            ManualBind::LuaRef result = complete( command.getText() );

            command.setText( result );
        }
        catch( ManualBind::LuaException &e )
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void Console::autoCompleteClear()
{
    ManualBind::LuaRef clearAuto( mL, "autoCompleteClear" );
    if( clearAuto.isFunction() )
    {
        try {
            clearAuto();
        }
        catch( ManualBind::LuaException &e )
        {
            std::cerr << e.what() << std::endl;
        }
    }
}
