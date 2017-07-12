#ifndef LB_RENDERLIST_H
#define LB_RENDERLIST_H

#include "LuaBinding.h"
#include "renderlist.h"

using namespace ManualBind;

struct RenderableBinding : public Binding<RenderableBinding,Renderable>
{
    static constexpr const char* class_name = "Renderable";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { nullptr, nullptr }
        };
        return members;
    }

    static bind_properties* properties()
    {
        static bind_properties properties[] = {
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static int create( lua_State* L )
    {
        return luaL_error( L, "Can not make an instance of an abstract class");
    }

};

struct RenderListBinding : public Binding<RenderListBinding,RenderList>
{
    static constexpr const char* class_name = "RenderList";

    static luaL_Reg* members()
    {
        static luaL_Reg members[] = {
            { "sort", sort },
            { "insert", insert },
            { "remove", remove },
            { "add", add },
            { nullptr, nullptr }
        };
        return members;
    }

    static bind_properties* properties()
    {
        static bind_properties properties[] = {
            { nullptr, nullptr, nullptr }
        };
        return properties;
    }

    static int create( lua_State* L )
    {
        RenderListPtr rl = std::make_shared<RenderList>();

        push( L, rl );

        return 1;
    }

    static int sort( lua_State* L )
    {
        RenderListPtr rl = fromStack( L, 1 );
        rl->sort();
        return 0;
    }

    static int insert( lua_State* L )
    {
        RenderListPtr rl = fromStack( L, 1 );
        LuaBindingUpCast( L, 2 );
        RenderablePtr r = RenderableBinding::fromStack( L, 2 );

        rl->insert( std::move(r) );

        return 0;
    }

    static int remove( lua_State* L )
    {
        RenderListPtr rl = fromStack( L, 1 );
        LuaBindingUpCast( L, 2 );
        RenderablePtr r = RenderableBinding::fromStack( L, 2 );

        rl->remove( r );

        return 0;
    }

    static int add( lua_State* L )
    {
        RenderListPtr rl = fromStack( L, 1 );
        LuaBindingUpCast( L, 2 );
        RenderablePtr r = RenderableBinding::fromStack( L, 2 );

        rl->add( std::move(r) );

        return 0;
    }
};

#endif // LB_RENDERLIST_H
