print 'Init Script'
package.path = './scripts/?.lua;' .. package.path
require( 'events' )
require( 'sprite' )
require( 'autocomplete' )
print( 'Completed' );
