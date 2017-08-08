#ifndef __COLOREDTEXT_H
#define __COLOREDTEXT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <string>

void test_colored_text( ALLEGRO_FONT* font );
// Returns width in pixels of the text.
int draw_colored_text( const ALLEGRO_FONT* font, ALLEGRO_COLOR start_color, float x, float y, int flags, const std::string& text );
// Removes color annotaions.
std::string strip_color_text( const std::string& text );

#endif // __COLOREDTEXT_H
