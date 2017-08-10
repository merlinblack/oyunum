#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <lua.hpp>
#include "LuaRef.h"

#include "renderlist.h"
#include "text.h"
#include "font.h"
#include "rectangle.h"
#include "bitmap.h"
#include "tileset.h"
#include "tilegrid.h"
#include "console.h"

#include "lb_renderlist.h"
#include "lb_text.h"
#include "lb_font.h"
#include "lb_color.h"

// 16:9
//#define SCREEN_W    640 
//#define SCREEN_H    360
#define SCREEN_W    800 
#define SCREEN_H    450

#define LUA_GAME_INDEX  "game"

class Game
{
    private:

    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* frameTimer;
    ALLEGRO_TIMER* scriptTimer;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_BITMAP* icon;

    RenderListPtr renderlist;

    FontPtr font;

    TextPtr fpsText;
    TextPtr spsText;
    TextPtr mouseText;

    ConsolePtr console;

    bool redraw;

    int width, height;
    float scaleX, scaleY;

    lua_State* L;

    bool shouldStop;

    public:

    Game() : display(nullptr), frameTimer(nullptr), eventQueue(nullptr), redraw(false), L(nullptr), shouldStop(false)
    {
    }

    void initialiseAllegro();

    bool createDisplay(); // Returns false on error.
    void registerEventSources();
    void initialiseLua();
    bool boot();
    void setScene();
    void updateMouseText();
    void resize();
    void run();
    void deregisterEventSources();
    void destroyDisplay();
    ~Game();

    // Lua routines.
    static int quit( lua_State* L );
};

#endif // GAME_H
