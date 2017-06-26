// Common stuff for the examples. Or debugging.
#ifndef DEBUG_H
#define DEBUG_H

#include <lua.hpp>

void dump( lua_State* L, const char* name );
void run( lua_State* L, const char* code );

#endif // DEBUG_H
