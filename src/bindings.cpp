#include "LuaBinding.h"
#include "lb_renderlist.h"
#include "lb_text.h"
#include "lb_font.h"
#include "lb_color.h"
#include "lb_bitmap.h"
#include "lb_tileset.h"
#include "lb_tilegrid.h"
#include "lb_dirlist.h"

const char* ColorBinding::prop_keys[] = { "r", "g", "b", "a", nullptr };
const char* BitmapBinding::prop_keys[] = { "x", "y", nullptr };

void register_all_classes( lua_State* L )
{
    FontBinding::register_class( L );
    ColorBinding::register_class( L );
    RenderableBinding::register_class( L );
    RenderListBinding::register_class( L );
    TextBinding::register_class( L );
    BitmapBinding::register_class( L );
    TileSetBinding::register_class( L );
    TileGridBinding::register_class( L );

    dirlist_register( L );
}
