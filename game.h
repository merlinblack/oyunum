#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include <lua.hpp>
#include "LuaRef.h"

#include "renderlist.h"
#include "text.h"
#include "rectangle.h"

#include "lb_renderlist.h"
#include "lb_text.h"
#include "lb_font.h"
#include "lb_color.h"

#define MAX_BUTTONS 4

class Game
{
    private:

    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* frameTimer;
    ALLEGRO_TIMER* scriptTimer;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* icon;
    
    RenderListPtr renderlist;

    TextPtr fpsText;
    TextPtr spsText;
    TextPtr mouseText;

    bool redraw;
    struct {
        bool buttons[MAX_BUTTONS+1];
        int x, y, z, w;
        int dx, dy, dz, dw;
    } mouse;

    int width, height;

    lua_State* L;

    public:

    Game() : display(nullptr), frameTimer(nullptr), eventQueue(nullptr), redraw(false)
    {
        for(int i=1; i < MAX_BUTTONS; i++ )
            mouse.buttons[i] = false;
    }

    void initialiseAllegro()
    {
        al_init();
        al_install_mouse();
        al_install_keyboard();
        al_init_image_addon();
        al_init_font_addon();
    }

    // Return false on error.
    bool createDisplay()
    {
        al_set_new_display_flags( ALLEGRO_RESIZABLE );
        display = al_create_display( 640, 400 );
        if( display == nullptr )
            return false;
        al_set_window_title( display, "My Game" );

        return true;
    }

    void registerEventSources()
    {
        frameTimer = al_create_timer( 1.0 / 60.0 );  // 60 FPS
        scriptTimer = al_create_timer( 1.0 / 100.0 ); // Update script called 100x per second
        eventQueue = al_create_event_queue();

        al_register_event_source( eventQueue, al_get_keyboard_event_source() );
        al_register_event_source( eventQueue, al_get_mouse_event_source() );
        al_register_event_source( eventQueue, al_get_display_event_source(display) );
        al_register_event_source( eventQueue, al_get_timer_event_source(frameTimer) );
        al_register_event_source( eventQueue, al_get_timer_event_source(scriptTimer) );

        al_start_timer( frameTimer );
        al_start_timer( scriptTimer );
    }

    void initialiseLua()
    {
        L = luaL_newstate();
        luaL_openlibs( L );

        FontBinding::register_class( L );
        ColorBinding::register_class( L );
        RenderableBinding::register_class( L );
        RenderListBinding::register_class( L );
        TextBinding::register_class( L );

        FontBinding::push( L, font );
        lua_setglobal( L, "fixed_font" );

        RenderListBinding::push( L, renderlist );
        lua_setglobal( L, "renderlist" );
    }

    bool boot()
    {
        renderlist = std::make_shared<RenderList>();

        initialiseAllegro();
        
        if( ! createDisplay() )
            return false;

        registerEventSources();

        font = al_load_font( "fixed_font.tga", 0, 0 );
        icon = al_load_bitmap( "icon.tga" );

        al_set_display_icon( display, icon );

        width = al_get_display_width( display );
        height = al_get_display_height( display );

        initialiseLua();

        redraw = true;

        return true;
    }

    void setScene()
    {
        using std::make_shared;

        RenderListPtr rl = make_shared<RenderList>();

        rl->add( make_shared<Rectangle>( 4, 4,  100, 30, 8, 8, al_map_rgba(58,68,115,200)));
        rl->add( make_shared<Rectangle>( 4, 34, 100, 60, 8, 8, al_map_rgba(58,68,15,200)));
        rl->add( make_shared<Rectangle>( 4, 64, 120, 90, 8, 8, al_map_rgba(58,28,75,200)));

        renderlist->add( std::move(rl) );
        
        fpsText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), 54, 8 );
        renderlist->add( fpsText );

        spsText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), 54, 38 );
        renderlist->add( spsText );

        mouseText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), 61, 68 );
        renderlist->add( mouseText );
        updateMouseText();

    }

    void updateMouseText()
    {
        char mouseStr[MAX_BUTTONS+1];

        for( int i=1; i < MAX_BUTTONS; i++ )
            mouseStr[i-1] = mouse.buttons[i] ? '^' : '_';

        mouseStr[MAX_BUTTONS-1] = 0;

        mouseText->clearText();
        mouseText << "Mouse: [" << mouseStr << "]";
    }

    void run()
    {
        if( luaL_dofile( L, "script.lua" ) )
        {
            std::cerr << lua_tostring( L, -1 ) << std::endl;
        }

        LuaRef updateScripts = LuaRef::getGlobal( L, "update" );

        int fps_accum = 0, fps = 0;
        double fps_time = 0; 

        int sps_accum = 0, sps = 0;
        double sps_time = 0;

        while( true )
        {
            ALLEGRO_EVENT event;
            al_wait_for_event( eventQueue, &event );

            if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
                break;

            if( event.type == ALLEGRO_EVENT_KEY_DOWN )
            {
                if( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE )
                    break;

                if( event.keyboard.keycode == ALLEGRO_KEY_Z )
                {
                    renderlist->remove( mouseText );
                }

                if( event.keyboard.keycode == ALLEGRO_KEY_X )
                {
                    renderlist->add( mouseText );
                }
            }

            if( event.type == ALLEGRO_EVENT_TIMER && event.timer.source == frameTimer )
            {
                double t = al_get_time();
                fps_accum++;

                if( t - fps_time >= 1 )
                {
                    fps = fps_accum;
                    fps_accum = 0;
                    fps_time = t;
                    fpsText->clearText();
                    fpsText << "FPS:  " << fps;
                }

                redraw = true;
            }

            if( event.type == ALLEGRO_EVENT_TIMER && event.timer.source == scriptTimer )
            {
                double t = al_get_time();
                sps_accum++;
                if( t - sps_time >= 1 )
                {
                    sps = sps_accum;
                    sps_accum = 0;
                    sps_time = t;
                    spsText->clearText();
                    spsText << "SPS: " << sps;
                }

                updateScripts( t );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ) {
                if( event.mouse.button < MAX_BUTTONS )
                    mouse.buttons[event.mouse.button] = true;
                updateMouseText();
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP ) {
                if( event.mouse.button < MAX_BUTTONS )
                    mouse.buttons[event.mouse.button] = false;
                updateMouseText();
            }

            if( event.type == ALLEGRO_EVENT_DISPLAY_RESIZE )
            {
                al_acknowledge_resize( display );

                width = al_get_display_width( display );
                height = al_get_display_height( display );

                redraw = true;
            }

            if( redraw && al_is_event_queue_empty( eventQueue ) )
            {
                redraw = false;

                al_clear_to_color( al_map_rgb( 83, 24, 24 ) );

                renderlist->render();

                al_flip_display();

            }
        }
    }

    void deregisterEventSources()
    {
        if( eventQueue )
        {
            al_destroy_event_queue( eventQueue );
            eventQueue = nullptr;
        }
    }

    void destroyDisplay()
    {
        if( display )
        {
            al_destroy_display( display );
            display = nullptr;
        }
    }

    ~Game()
    {
        lua_close( L );
        deregisterEventSources();
        destroyDisplay();
    }
};

#endif // GAME_H
