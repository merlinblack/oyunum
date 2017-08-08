#ifndef CONSOLE_H
#define CONSOLE_H

#include <lua.hpp>
#include <string>
#include <list>
#include <memory>
#include "renderlist.h"
#include "font.h"
#include "editstring.h"
#include "luainterpreter.h"

using StringVector = std::list<std::string>;

class Console : public Renderable
{
    StringVector mLines;
    StringVector mHistory;
    StringVector::iterator mHistoryLine;
    EditString command;
    LuaInterpreter interpreter;
    FontPtr mFont;
    lua_State* mL;
    int width;
    bool visible;

    int mStartLine;
    double cursorBlinkTime;
    bool cursorBlink;

    public:

    Console( lua_State* L, FontPtr font, int _width );

    ~Console();

    bool isVisible();
    void toggleVisibility();
    void clear();
    void resume();

    static int luaCommand( lua_State* L );
    static int luaPrint( lua_State* L );

    bool injectKeyPress( const ALLEGRO_EVENT& event );

    void addLine( const std::string& line );
    void print( const std::string& text );

    void render();

    inline void pageUp();
    inline void pageDown();

    void execute();

    void historyUp();
    void historyDown();
    void historyAdd( const std::string& cmd );

    void autoComplete();
    void autoCompleteClear();
};

using ConsolePtr = std::shared_ptr<Console>;

#endif // CONSOLE_H

