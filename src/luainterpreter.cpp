/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * -------------------------------------------------------------------------------
 */


#include "luainterpreter.h"
#include <sstream>

using std::string;
using std::stringstream;

LuaInterpreter::LuaInterpreter(lua_State *L)
{
    init( L );
}

void LuaInterpreter::init(lua_State *L) 
{
    mL = L;
    mState = LI_READY;
    mFirstLine = true;
    mCurrentStatement.clear();
    mPrompt = LI_PROMPT;
}

// Insert (another) line of text into the interpreter.
LuaInterpreter::State LuaInterpreter::insertLine( std::string line  )
{
    if( mFirstLine && line.substr(0,1) == "=" )
    {
        line = "return " + line.substr(1, line.length()-1 );
    }

    mCurrentStatement += " ";
    mCurrentStatement += line;
    mFirstLine = false;

    mState = LI_READY;

    if( luaL_loadstring( mL, mCurrentStatement.c_str() ) )
    {
        string error( lua_tostring( mL, -1 ) );
        lua_pop( mL, 1 );

        // If the error is not a syntax error caused by not enough of the
        // statement been yet entered...
        if( error.substr( error.length()-5, 5 ) != "<eof>" )
        {
            // Print error
            lua_getglobal( mL, "print" );
            lua_pushstring( mL, error.c_str() );
            lua_pcall( mL, 1, 0, 0 );
            mCurrentStatement.clear();
            mState = LI_ERROR;

            return mState;
        }
        // Otherwise...
        else
        {
            // Try putting "()" on the end.  This allows commands such as 'quit' to invoke 'quit()'
            if( luaL_loadstring( mL, (mCurrentStatement + "()").c_str() ) )
            {
                // Didn't work.  Remove error message.
                lua_pop( mL, 1 );

                // Secondary prompt
                mPrompt = LI_PROMPT2;

                mState = LI_NEED_MORE_INPUT;

                return mState;
            }
        }
    }

    // The statement compiled correctly, get a new thread, grab a reference, and run it.

    lua_State *thread = lua_newthread( mL );

    // Grab a reference
    int mCoroutineRef = luaL_ref( mL, LUA_REGISTRYINDEX );

    // Move compiled chunk to thread.
    lua_xmove( mL, thread, 1 );

    // Run!
    int nResults;
    int ret = lua_resume( thread, mL, 0, &nResults );

    switch( ret )
    {
        default:
            // The error message (if any) will be added to the output as part
            // of the stack reporting.
            luaL_unref( mL, LUA_REGISTRYINDEX, mCoroutineRef );
            // Zap nil return value and replace with message.
            if( lua_isnil( thread, 1 ) )
            {
                lua_pushstring( thread, "Error: " );
                lua_replace( thread, 1 );
            }
            // else leave it alone.

            mState = LI_ERROR;
            break;
        case LUA_YIELD:
            // Chunk has yielded.
            mCoroutines.push_back( mCoroutineRef );
            mState = LI_YIELDING;
            break;
        case 0:
            // Ran to completion.
            luaL_unref( mL, LUA_REGISTRYINDEX, mCoroutineRef );
            break;
    }

    mCurrentStatement.clear();
    mFirstLine = true;

    reportStack( thread );

    // Remove any yielded resutls.
    lua_pop(thread, nResults);

    mPrompt = LI_PROMPT;

    return mState;
}

LuaInterpreter::State LuaInterpreter::resume()
{
    if( mCoroutines.empty() == true )
    {
        return mState;  // No coroutines to run.
    }

    CoroutineList::iterator coroutine = mCoroutines.begin();

    while( coroutine != mCoroutines.end() )
    {

        lua_rawgeti( mL, LUA_REGISTRYINDEX, *coroutine );    // Get the thread back...
        lua_State* thread = lua_tothread( mL, -1 );
        lua_pop( mL, 1 );

        int nResults;
        int ret = lua_resume( thread, mL, 0, &nResults );

        switch( ret )
        {
            default:
                // The error message (if any) will be added to the output as part of the stack reporting.
                luaL_unref( mL, LUA_REGISTRYINDEX, *coroutine );
                mState = LI_ERROR;
                coroutine = mCoroutines.erase( coroutine );
                break;
            case LUA_YIELD:
                // Chunk has yielded.
                mState = LI_YIELDING;
                coroutine++;
                break;
            case 0:
                // Ran to completion.
                luaL_unref( mL, LUA_REGISTRYINDEX, *coroutine );
                mState = LI_READY;
                coroutine = mCoroutines.erase( coroutine );
                break;
        }

        reportStack( thread );

        // Remove any yielded resutls.
        lua_pop(thread, nResults);
    }

    return mState;
}

void LuaInterpreter::reportStack( lua_State* thread )
{
    // Report stack contents
    // In the case of a yielded chunk these are the parameters to yield.
    if( mState == LI_ERROR ) {
      lua_getglobal( thread, "print" );
      lua_pushliteral( thread, "Error" );
      lua_pushvalue( thread, -3 );
      lua_pcall(thread, 2, 0, 0);
      lua_settop( thread, 0 );
      mState = LI_READY;
    }
    else if ( lua_gettop(thread) > 0)
    {
      string stack = dumpstack_str( thread );
      lua_settop( thread, 0 );
      lua_getglobal( thread, "print" );
      lua_pushstring( thread, stack.c_str() );
      lua_pcall(thread, 1, 0, 0);
    }
}

const string dumpstack_str(lua_State* L )
{
    static stringstream ss;
    int i;
    int top = lua_gettop(L);

    ss.str("\n");

    for (i = 1; i <= top; i++)
    {
        ss << i << "> " << luaL_typename(L,i) << " - ";
        switch (lua_type(L, i))
        {
            case LUA_TNUMBER:
                ss << lua_tonumber(L,i) << "\n";
            break;
            case LUA_TUSERDATA:
            case LUA_TSTRING:
                ss << luaL_tolstring(L, i, nullptr) << "\n";
                lua_pop(L,1);
                break;
            case LUA_TBOOLEAN:
                ss << (lua_toboolean(L, i) ? "true" : "false") << "\n";
                break;
            case LUA_TNIL:
                ss << "nil\n";
                break;
            default:
                ss << std::hex << "0x" << lua_topointer(L,i) << "\n";
                break;
        }
    }
    return ss.str();
}

void dumpstack (lua_State *L, const char *message)
{
    printf("thread -- %p\n", L );
    printf("dumpstack -- %s\n",message);
    printf("%s\n", dumpstack_str( L ).c_str() );
    printf("dumpstack -- END\n");
}

