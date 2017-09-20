#include "game.h"
#include "git_versioning.h"

#include <iostream>
using std::cout;
using std::endl;

// in binding.cpp
void register_all_classes( lua_State* L );
// in colortext.cpp
void test_colored_text( ALLEGRO_FONT* font );

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

    resize();

    return true;
}

void Game::resize()
{
    width = al_get_display_width( display );
    height = al_get_display_height( display );

    scaleX = width / (float)SCREEN_W;
    scaleY = height / (float)SCREEN_H;

    cout << "X scale: " << scaleX << endl;
    cout << "Y scale: " << scaleY << endl;
    cout << endl;

    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_scale_transform(&trans, scaleX, scaleY);
    al_use_transform(&trans);
}

void Game::registerEventSources()
{
    frameTimer = al_create_timer( 1.0 / 30.0 );  // 60 FPS
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
    lua_register( L, "setTimerDivider", Game::setTimerDivider );

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
    console->print( std::string( "Está es una prueba\n^red^Bu bir test\nOne\t1\nTwo\t2\nÜç\t3\nDört\t4" ) );
    console->print( std::string( "^yellow^" GIT_REPO_VERSION_STR ));

    return true;
}

void Game::setScene()
{
    using std::make_shared;

    RenderListPtr rl = make_shared<RenderList>();

    rl->add( make_shared<Rectangle>( SCREEN_W-480, SCREEN_H-35, SCREEN_W-330, SCREEN_H-5, 8, 8, al_map_rgba(58,68,115,200)));
    rl->add( make_shared<Rectangle>( SCREEN_W-320, SCREEN_H-35, SCREEN_W-170, SCREEN_H-5, 8, 8, al_map_rgba(58,68,115,200)));
    rl->add( make_shared<Rectangle>( SCREEN_W-160, SCREEN_H-35, SCREEN_W-10,  SCREEN_H-5, 8, 8, al_map_rgba(58,68,115,200)));

    fpsText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), SCREEN_W-85, SCREEN_H-30 );
    rl->add( fpsText );

    spsText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), SCREEN_W-245, SCREEN_H-30 );
    rl->add( spsText );

    memText = make_shared<Text>( font, al_map_rgb( 255, 255, 255 ), SCREEN_W-405, SCREEN_H-30 );
    rl->add( memText );

    rl->setOrder( 254 );

    renderlist->insert( std::move(rl) );
}

void Game::run()
{
    if( luaL_dofile( L, "scripts/main.lua" ) )
    {
        std::cerr << lua_tostring( L, -1 ) << std::endl;
        return;
    }

    LuaRef luaUpdate( L, "update" );
    LuaRef luaKeyEvent( L, "keyEvent" );
    LuaRef luaMouseEvent( L, "mouseEvent" );

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

    int fps_accum = 0;
    int fps_worst = 1000;
    int fps_best = 0;
    int sps_accum = 0;
    int sps_worst = 1000;
    int sps_best = 0;
    double update_time = al_get_time(); 
    double t;

    bool redraw = false;
    bool scriptrun = false;

    while( ! shouldStop )
    {
        ALLEGRO_EVENT event;
        al_wait_for_event( eventQueue, &event );

        t = al_get_time();

        if( t - update_time >= 1 ) // Every second ...
        {
            update_time = t;

            if( fps_best < fps_accum ) { fps_best = fps_accum; }
            if( fps_worst > fps_accum ) { fps_worst = fps_accum; }
            fpsText->clearText();
            fpsText << "FPS: " << fps_accum << "/" << fps_best << "/" << fps_worst;
            fps_accum = 0;

            if( sps_best < sps_accum ) { sps_best = sps_accum; }
            if( sps_worst > sps_accum ) { sps_worst = sps_accum; }
            spsText->clearText();
            spsText << "SPS: " << sps_accum << "/" << sps_best << "/" << sps_worst;
            sps_accum = 0;

            memText->clearText();
            memText << "Lua Mem " << lua_gc( L, LUA_GCCOUNT, 0 ) << "kb";
        }

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
                        LuaRef quit( L, "quit" );
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

                luaKeyEvent( al_get_time(), event.keyboard.keycode, event.keyboard.unichar );
            }

            if( event.type == ALLEGRO_EVENT_TIMER && event.timer.source == frameTimer )
            {
                redraw = true;
            }

            if( event.type == ALLEGRO_EVENT_TIMER && event.timer.source == scriptTimer )
            {
                scriptrun = true;
            }

            if( scriptrun && al_is_event_queue_empty( eventQueue ) )
            {
                sps_accum++;
                luaUpdate( t );
                console->resume();
                scriptrun = false;
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ) {
                luaMouseEvent( al_get_time(), "down", event.mouse.button );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP ) {
                luaMouseEvent( al_get_time(), "up", event.mouse.button );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_WARPED ) {
                luaMouseEvent( al_get_time(), "move", event.mouse.button,
                        event.mouse.x / scaleX,
                        event.mouse.y / scaleY,
                        event.mouse.z,
                        event.mouse.dx / scaleX,
                        event.mouse.dy / scaleY,
                        event.mouse.dz
                        );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY ) {
                luaMouseEvent( al_get_time(), "enter", event.mouse.button,
                        event.mouse.x / scaleX,
                        event.mouse.y / scaleY,
                        event.mouse.z
                        );
            }

            if( event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY ) {
                luaMouseEvent( al_get_time(), "leave", event.mouse.button,
                        event.mouse.x / scaleX,
                        event.mouse.y / scaleY,
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
            fps_accum++;

            redraw = false;

            al_clear_to_color( al_map_rgb( 43, 73, 46 ) );

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

int Game::setTimerDivider( lua_State *L )
{
    static const char* keys[] = { "fps", "sps", nullptr };
    lua_getfield( L, LUA_REGISTRYINDEX, LUA_GAME_INDEX );
    Game* self = static_cast<Game*>(lua_touserdata( L, -1 ));

    int which = luaL_checkoption( L, 1, nullptr, keys );

    float divider = luaL_checknumber( L, 2 );

    switch( which )
    {
        case 0:
            al_set_timer_speed( self->frameTimer, 1.0 / divider );
            break;
        case 1:
            al_set_timer_speed( self->scriptTimer, 1.0 / divider );
            break;
        default:
            return luaL_error( L, "First parameter should be either 'fps' or 'sps'." );
            break;
    }

    return 0;
}
