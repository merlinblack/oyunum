print 'Init Script'
package.path = './scripts/?.lua;./scripts/zzlib/?.lua;' .. package.path
require( 'tasks' )
require( 'events' )
require( 'sprite' )
require( 'autocomplete' )
require( 'misc' )
require( 'gui.gui' )
print( 'Completed' );

function gensprites()
    dofile 'scripts/gensprites.lua'
end

function rungame()
    dofile 'scripts/magic_cliffs.lua'
end

function showgarbage()
    local mt = { __gc = function(o) 
        print( '+++ Garbage Collection +++' )
        setmetatable( {}, getmetatable( o ) )
    end }
    setmetatable( {}, mt )
end

function sps( x )
    setTimerDivider('sps',x);
end

function fps( x )
    setTimerDivider('fps',x);
end
