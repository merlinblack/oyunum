#ifndef LB_BITMAP_H
#define LB_BITMAP_H

#include "bitmap.h"
#include "LuaBinding.h"

struct BitmapBinding : public Binding<BitmapBinding,Bitmap>
{
    static constexpr const char* class_name = "Bitmap";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "loadFile", loadFile },
            { "getSubBitmap", getSubBitmap },
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
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static int create( lua_State* L )
    {
        BitmapPtr b = std::make_shared<Bitmap>();

        push( L, b );

        return 1;
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

        int x = luaL_checkinteger( L, 2 );
        int y = luaL_checkinteger( L, 3 );
        int w = luaL_checkinteger( L, 4 );
        int h = luaL_checkinteger( L, 5 );

        BitmapPtr s = b->getSubBitmap( x, y, w, h );

        push( L, s );

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
                b->setX( luaL_checkinteger( L, 3 ) );
                break;

            case 1:
                b->setY( luaL_checkinteger( L, 3 ) );
                break;
        }

        return 0;
    }
};

#endif //LB_BITMAP_H