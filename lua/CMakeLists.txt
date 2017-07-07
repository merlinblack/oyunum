set (${PROJECT_NAME}_LUA_C_FLAGS "-O3" CACHE STRING "${PROJECT_NAME} Extra C Flags for Lua" )

# Extra C flags for Lua
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${${PROJECT_NAME}_LUA_C_FLAGS}")
# Remove debug if it is there.
string(REPLACE "-g" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

if(WIN32)
    set (Lua_LIBRARIES lua PARENT_SCOPE )
    set (Lua_LIBRARIES lua )
elseif(UNIX)
    set(Lua_LIBRARIES lua dl m PARENT_SCOPE )
    set(Lua_LIBRARIES lua dl m )
    set(Lua_EXEC_LIBRARIES readline)
    add_definitions(-DLUA_USE_LINUX)
endif()

# define the lua source files
file(GLOB Lua_LIB_SRCS lua-5.3.4/src/*.c)
file(GLOB Lua_Interpreter lua-5.3.4/src/lua.c)
file(GLOB Lua_Compiler    lua-5.3.4/src/luac.c)
list(REMOVE_ITEM Lua_LIB_SRCS ${Lua_Interpreter} ${Lua_Compiler})

set (Lua_INCLUDE_DIRS lua/lua-5.3.4/src PARENT_SCOPE )

# create the library
add_library (lua ${Lua_LIB_SRCS})

# create the lua executable and link it to the lib
add_executable (luaexec ${Lua_Interpreter})
target_link_libraries (luaexec ${Lua_LIBRARIES} ${Lua_EXEC_LIBRARIES})

# name the executable lua just like the library
set_target_properties(luaexec PROPERTIES OUTPUT_NAME lua)

# create the luac
add_executable (luac ${Lua_Compiler})
target_link_libraries (luac ${Lua_LIBRARIES} ${Lua_EXEC_LIBRARIES})
