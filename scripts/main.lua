print 'Init Script'
package.path = './scripts/?.lua;' .. package.path
require( 'tasks' )
require( 'events' )
require( 'sprite' )
require( 'autocomplete' )
require( 'misc' )
print( 'Completed' );

function gensprites()
    dofile 'scripts/gensprites.lua'
end

function rungame()
    dofile 'scripts/magic_cliffs.lua'
end
