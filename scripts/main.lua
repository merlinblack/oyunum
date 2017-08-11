print 'Init Script'
package.path = './scripts/?.lua;' .. package.path
require( 'tasks' )
require( 'events' )
require( 'sprite' )
require( 'autocomplete' )
print( 'Completed' );
