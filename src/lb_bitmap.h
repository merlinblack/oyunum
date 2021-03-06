#ifndef LB_BITMAP_H
#define LB_BITMAP_H

#include "bitmap.h"
#include "LuaBinding.h"
#include "lb_renderlist.h"

using namespace ManualBind;

struct BitmapBinding : public Binding<BitmapBinding,Bitmap>
{
    static constexpr const char* class_name = "Bitmap";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "create", createNew },
            { "blit", blit },
            { "loadFile", loadFile },
            { "getSubBitmap", getSubBitmap },
            { "setBitmap", setBitmap },
            { "saveBitmap", saveBitmap },
            { "__upcast", upcast },
            { nullptr, nullptr }
        };
        return members;
    }

    static bind_properties* properties()
    {
        static bind_properties properties[] = {
            { "x", get_xy, set_xy },
            { "y", get_xy, set_xy },
            { "w", get_wh, nullptr },
            { "h", get_wh, nullptr },
            { "scale", get_scale, set_scale },
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static void setExtraMeta( lua_State* L )
    {
        lua_pushinteger( L, ALLEGRO_MIN_LINEAR );
        lua_setfield( L, -2, "ALLEGRO_MIN_LINEAR" );
        lua_pushinteger( L, ALLEGRO_MAG_LINEAR );
        lua_setfield( L, -2, "ALLEGRO_MAG_LINEAR" );
        return;
    }

    static int create( lua_State* L )
    {
        BitmapPtr b = std::make_shared<Bitmap>();

        const char* filename = luaL_optstring( L, 1, nullptr );
        int flags = luaL_optinteger( L, 2, 0 );

        if( filename != nullptr )
            b->loadFromFile( filename, flags );

        push( L, b );

        return 1;
    }

    static int createNew( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );
        int w = luaL_checknumber( L, 2 );
        int h = luaL_checknumber( L, 3 );
        int flags = luaL_optinteger( L, 4, 0 );

        b->create( w, h, flags );

        return 0;
    }

    static int blit( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );
        BitmapPtr o = fromStack( L, 2 );
        float x = luaL_checknumber( L, 3 );
        float y = luaL_checknumber( L, 4 );
        float scale = luaL_checknumber( L, 5 );

        b->blit( o, x, y, scale );

        return 0;
    }

    static int loadFile( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        std::string filename( lua_tostring( L, 2 ) );

        lua_pushboolean( L, b->loadFromFile( filename ) );

        return 1;
    }

    static int getSubBitmap( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        int x = (int)luaL_checknumber( L, 2 );
        int y = (int)luaL_checknumber( L, 3 );
        int w = (int)luaL_checknumber( L, 4 );
        int h = (int)luaL_checknumber( L, 5 );

        BitmapPtr s = b->getSubBitmap( x, y, w, h );

        push( L, s );

        return 1;
    }

    static int setBitmap( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );
        BitmapPtr n = fromStack( L, 2 );

        b->setBitmap( n );

        return 0;
    }

    static int saveBitmap( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        std::string filename( lua_tostring( L, 2 ) );

        lua_pushboolean( L, b->saveToFile( filename ) );

        return 1;
    }

    static int upcast( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        RenderablePtr r = std::dynamic_pointer_cast<Renderable>( b );

        RenderableBinding::push( L, r );

        return 1;
    }

    static const char* prop_keys[];

    static int get_xy( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        int which = luaL_checkoption( L, 2, NULL, BitmapBinding::prop_keys );

        switch( which )
        {
            case 0:
                lua_pushinteger( L, b->getX() );
                break;

            case 1:
                lua_pushinteger( L, b->getY() );
                break;
        }

        return 1;
    }

    static int set_xy( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        int which = luaL_checkoption( L, 2, NULL, BitmapBinding::prop_keys );

        switch( which )
        {
            case 0:
                b->setX( luaL_checknumber( L, 3 ) );
                break;

            case 1:
                b->setY( luaL_checknumber( L, 3 ) );
                break;
        }

        return 0;
    }

    static int get_scale( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        lua_pushnumber( L, b->getScale() );

        return 1;
    }

    static int set_scale( lua_State* L )
    {
        BitmapPtr b = fromStack( L, 1 );

        float scale = luaL_checknumber( L, 3 );

        b->setScale( scale );

        return 0;
    }

    static int get_wh( lua_State* L )
    {
        static const char* keys[] = { "w", "h", nullptr };

        BitmapPtr b = fromStack( L, 1 );

        int which = luaL_checkoption( L, 2, nullptr, keys ); 

        switch( which )
        {
            case 0:
                lua_pushnumber( L, b->getWidth() );
                break;
            case 1:
                lua_pushnumber( L, b->getHeight() );
                break;
        }

        return 1;
    }

};

#endif //LB_BITMAP_H
