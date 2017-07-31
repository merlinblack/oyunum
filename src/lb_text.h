#ifndef LB_TEXT_H
#define LB_TEXT_H

#include "LuaBinding.h"
#include "lb_renderlist.h"
#include "lb_font.h"
#include "lb_color.h"
#include "text.h"

using namespace ManualBind;

struct TextBinding : public Binding<TextBinding,Text>
{
    static constexpr const char* class_name = "Text";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "__upcast", upcast },
            { "setXY", setXY },
            { nullptr, nullptr }
        };
        return members;
    }

    static bind_properties* properties()
    {
        static bind_properties properties[] = {
            { "text", getText, setText },
            { "color", getColor, setColor },
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static int create( lua_State* L )
    {
        std::string text = luaL_checkstring( L, 1 );
        FontPtr font = FontBinding::fromStack( L, 2 );
        ALLEGRO_COLOR color = ColorBinding::fromStack( L, 3 );
        int x = luaL_checkinteger( L, 4 );
        int y = luaL_checkinteger( L, 5 );

        TextPtr t = std::make_shared<Text>( std::move(text), font, color, x, y);

        push( L, t );

        return 1;
    }

    static int getText( lua_State* L )
    {
        TextPtr t = fromStack( L, 1 );

        lua_pushstring( L, t->getText().c_str() );

        return 1;
    }

    static int setText( lua_State* L )
    {
        TextPtr t = fromStack( L, 1 );
        std::string text = luaL_checkstring( L, 3 );

        t->setText( std::move(text) );

        return 0;
    }

    static int getColor( lua_State* L )
    {
        TextPtr t = fromStack( L, 1 );

        ColorBinding::push( L, t->getColor() );

        return 1;
    }

    static int setColor( lua_State* L )
    {
        TextPtr t = fromStack( L, 1 );
        ALLEGRO_COLOR c = ColorBinding::fromStack( L, 3 );

        t->setColor( c );

        return 0;
    }

    static int setXY( lua_State* L )
    {
        TextPtr t = fromStack( L, 1 );
        int x = luaL_checkinteger( L, 2 );
        int y = luaL_checkinteger( L, 3 );

        t->setXY( x, y );

        return 0;
    }

    int static upcast( lua_State* L )
    {
        TextPtr t = fromStack( L, 1 );

        RenderablePtr r = std::dynamic_pointer_cast<Renderable>( t );

        RenderableBinding::push( L, r );

        return 1;
    }

};

#endif //LB_TEXT_H
