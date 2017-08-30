require 'packatlas'

packatlas( 'data/sprites/gui', 'data/sprites', 'gui', 512, 512, 1, 5 ):process()
packatlas( 'data/sprites/templerun', 'data/sprites', 'templerun', 768, 768, .18, 0 ):process()
packatlas( 'data/sprites/adventuregirl', 'data/sprites', 'adventuregirl', 768, 1024, .18, 0 ):process()
packatlas( 'data/sprites/ninjagirlnew', 'data/sprites', 'ninjagirlnew', 1024, 1024, .18, 0 ):process()
packatlas( 'data/sprites/ninjaadventurenew', 'data/sprites', 'ninjaadventurenew', 1024, 1024, .18, 0 ):process()
packatlas( 'data/sprites/jackfree', 'data/sprites', 'jackfree', 1024, 1024, .18, 0 ):process()
packatlas( 'data/sprites/cards', 'data/sprites', 'cards', 1024, 1024, 1, 5 ):process()

print( 'Finished generating sprite atlases.' )
