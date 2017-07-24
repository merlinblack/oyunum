#include "game.h"
#include "git_versioning.h"

#include <iostream>
using std::cout;
using std::endl;

// in binding.cpp
void register_all_classes( lua_State* L );

void Game::initialiseAllegro()
{
    al_init();
    al_install_mouse();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
}

bool Game::createDisplay()
{
    al_set_new_display_flags( ALLEGRO_RESIZABLE );
    display = al_create_display( SCREEN_W, SCREEN_H );
    if( display == nullptr )
        return false;
    al_set_window_title( display, "My Game" );
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    resize();

    return true;
}

void Game::resize()
{
    width = al_get_display_width( display );
    height = al_get_display_height( display );

    float sx = width / (float)SCREEN_W;
    float sy = height / (float)SCREEN_H;

    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_scale_transform(&trans, sx, sy);
    al_use_transform(&trans);
}

void Game::registerEventSources()
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

void Game::initialiseLua()
{
    L = luaL_newstate();
    luaL_openlibs( L );

    register_all_classes( L );

    lua_pushlightuserdata( L, this );
    lua_setfield( L, LUA_REGISTRYINDEX, LUA_GAME_INDEX );

    lua_register( L, "quit", Game::quit );

    FontBinding::push( L, font );
    lua_setglobal( L, "fixed_font" );

    RenderListBinding::push( L, renderlist );
    lua_setglobal( L, "renderlist" );

    LuaRef info = LuaRef::newTable( L );
    info["width"] = SCREEN_W;
    info["height"] = SCREEN_H;
    info["gitver"] = GIT_REPO_VERSION_STR; 
    info.push();
    lua_setglobal( L, "info" );
}

bool Game::boot()
{
    renderlist = std::make_shared<RenderList>();

    initialiseAllegro();

    if( ! createDisplay() )
        return false;

    registerEventSources();

    font = std::make_shared<Font>( "data/fixed_font.tga", 0, 0 );

    icon = al_load_bitmap( "data/icon.tga" );

    if( !font || !icon )
        return false;

    al_set_display_icon( display, icon );

    initialiseLua();
    
    FontPtr console_font = std::make_shared<Font>( "data/liberation_mono/LiberationMono-Bold.ttf", -16, ALLEGRO_TTF_NO_KERNING );

    console = std::make_shared<Console>( L, console_font, SCREEN_W );
    console->setOrder(255);
    console->print( std::string(  "Está es una prueba\nTeşekker edirim\nOne\t1\nTwo\t2\nÜç\t3\nDört\t4" ) );

    redraw = true;

    return true;
}

void Game::setScene()
{
    using std::make_shared;

    RenderListPtr rl = make_shared<RenderList>();

    rl->add( make_shared<Rectangle>( 4, 4,  100, 30, 8, 8, al_map_rgba(58,68,115,200)));
    rl->add( make_shared<Rectangle>( 4, 34, 100, 60, 8, 8, al_map_rgba(58,68,15,200)));
    rl->add( make_shared<Rectangle>( 4, 64, 120, 90, 8, 8, al_map_rgba(58,28,75,200)));

    fpsText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), 54, 8 );
    rl->add( fpsText );

    spsText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), 54, 38 );
    rl->add( spsText );

    mouseText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), 61, 68 );
    rl->add( mouseText );

    rl->setOrder( 254 );

    renderlist->insert( std::move(rl) );

    updateMouseText();
}

void Game::updateMouseText()
{
    char mouseStr[MAX_BUTTONS+1];

    for( int i=1; i < MAX_BUTTONS; i++ )
        mouseStr[i-1] = mouse.buttons[i] ? '^' : '_';

    mouseStr[MAX_BUTTONS-1] = 0;

    mouseText->clearText();
    mouseText << "Mouse: [" << mouseStr << "]";
}

void Game::run()
{
    if( luaL_dofile( L, "scripts/main.lua" ) )
    {
        std::cerr << lua_tostring( L, -1 ) << std::endl;
        return;
    }

    LuaRef luaUpdate     = LuaRef::getGlobal( L, "update" );
    LuaRef luaKeyEvent   = LuaRef::getGlobal( L, "keyEvent" );
    LuaRef luaMouseEvent = LuaRef::getGlobal( L, "mouseEvent" );

    // Check scripts are sane.
    if( ! luaUpdate.isFunction() )
    {
        std::cerr << "Lua update function is not defined." << std::endl;
        shouldStop = true;
    }
    if( ! luaKeyEvent.isFunction() )
    {
        std::cerr << "Lua keyEvent function is not defined." << std::endl;
        shouldStop = true;
    }
    if( ! luaMouseEvent.isFunction() )
    {
        std::cerr << "Lua mouseEvent function is not defined." << std::endl;
        shouldStop = true;
    }

    int fps_accum = 0, fps = 0;
    double fps_time = 0; 

    int sps_accum = 0, sps = 0;
    double sps_time = 0;

    while( ! shouldStop )
    {
        ALLEGRO_EVENT event;
        al_wait_for_event( eventQueue, &event );

        if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
            break;

        try {
            if( event.type == ALLEGRO_EVENT_KEY_CHAR )
            {
                if( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE )
                {
                    if( console->isVisible() )
                    {
                        console->toggleVisibility();
                        continue;
                    }
                    else
                    {
                        // We grab it here, as 'quit' may change depending
                        // on what is happening.
                        LuaRef quit = LuaRef::getGlobal( L, "quit" );
                        if( quit.isFunction() ) {
                            quit();
                        }
                        continue;
                    }
                }

                if( console->isVisible() )
                {
                    if( console->injectKeyPress( event ) )
                    {
                        redraw = true;
                        continue;
                    }
                }
                else
                {
                    if( event.keyboard.keycode == ALLEGRO_KEY_TILDE )
                    {
                        console->toggleVisibility();
                        continue;
                    }
                }

                luaKeyEvent( event.keyboard.keycode, event.keyboard.unichar );
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

                luaUpdate( t );
                console->resume();
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ) {
                if( event.mouse.button < MAX_BUTTONS )
                    mouse.buttons[event.mouse.button] = true;
                updateMouseText();
                luaMouseEvent( "down", event.mouse.button );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP ) {
                if( event.mouse.button < MAX_BUTTONS )
                    mouse.buttons[event.mouse.button] = false;
                updateMouseText();
                luaMouseEvent( "up", event.mouse.button );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_WARPED ) {
                luaMouseEvent( "move", event.mouse.button,
                        event.mouse.x,
                        event.mouse.y,
                        event.mouse.z,
                        event.mouse.dx,
                        event.mouse.dy,
                        event.mouse.dz
                        );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY ) {
                luaMouseEvent( "enter", event.mouse.button,
                        event.mouse.x,
                        event.mouse.y,
                        event.mouse.z
                        );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY ) {
                luaMouseEvent( "leave", event.mouse.button,
                        event.mouse.x,
                        event.mouse.y,
                        event.mouse.z
                        );
            }
        }
        catch( LuaException &e )
        {
            std::cerr << e.what() << std::endl;
        }

        if( event.type == ALLEGRO_EVENT_DISPLAY_RESIZE )
        {
            al_acknowledge_resize( display );

            resize();

            redraw = true;
        }

        if( redraw && al_is_event_queue_empty( eventQueue ) )
        {
            redraw = false;

            al_clear_to_color( al_map_rgb( 83, 24, 24 ) );

            renderlist->render();

            console->render();

            al_flip_display();
        }
    }
}

void Game::deregisterEventSources()
{
    if( eventQueue )
    {
        al_destroy_event_queue( eventQueue );
        eventQueue = nullptr;
    }
}

void Game::destroyDisplay()
{
    if( display )
    {
        al_destroy_display( display );
        display = nullptr;
    }
}

Game::~Game()
{
    console.reset();
    if( L ) {
        lua_close( L );
    }
    deregisterEventSources();
    destroyDisplay();
}

int Game::quit( lua_State *L )
{
    lua_getfield( L, LUA_REGISTRYINDEX, LUA_GAME_INDEX );
    Game* self = static_cast<Game*>(lua_touserdata( L, -1 ));

    self->shouldStop = true;

    return 0;
}
