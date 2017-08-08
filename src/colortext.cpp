#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>

#include <iostream>
using std::endl;
using std::cout;

struct TextElement
{
    std::string str;
    ALLEGRO_COLOR color;
    int width;
};

using TextElementVector = std::vector<TextElement>;

ALLEGRO_COLOR getColorFromString( const std::string& color, ALLEGRO_COLOR reset )
{
    float r, g, b;
    
    if( color[0] == '!' ) {
        return reset;
    }

    if( color[0] == '#' ) {
        al_color_html_to_rgb( color.c_str(), &r, &g, &b );
    } else {
        al_color_name_to_rgb( color.c_str(), &r, &g, &b );
    }

    return al_map_rgb_f( r, g, b );
}

int draw_colored_text( const ALLEGRO_FONT* font, ALLEGRO_COLOR start_color, float x, float y, int flags, const std::string& text )
{
    TextElementVector list;
    std::string currentText;
    std::string colorText;
    ALLEGRO_COLOR color = start_color;
    bool mode = false;
    int width = 0;

    for( auto i = text.begin(); i != text.end(); i++ )
    {
        if( *i == '^' ) {
            auto j = i+1;
            if( j == text.end() ) // ^ at the end, ignore
                continue;
            if( *j == '^' ) {    // ^^ - put in '^' and skip to next char
                currentText.push_back( '^' );
                i = j;
                continue;
            }
            // single ^. Process and switch mode.
            if( mode ) {
                // Get color from color string.
                color = getColorFromString( colorText, start_color );
                colorText.clear();
            } else {
                // Put text into list.
                if( ! currentText.empty() ) {
                    list.push_back( TextElement{ currentText, color, 0 } );
                    currentText.clear();
                }
            }
            mode = ! mode;
        } else {
            if( mode ) {
                colorText.push_back( *i );
            } else {
                currentText.push_back( *i );
            }
        }
    }

    list.push_back( TextElement{ currentText, color, 0 } );

    for( auto& te: list ) {
        te.width = al_get_text_width( font, te.str.c_str() );
        width += te.width;
    }

    // Adjust x according to flags
    if( flags == ALLEGRO_ALIGN_RIGHT ) {
        x -= width;
    }
    
    if( flags == ALLEGRO_ALIGN_CENTRE ) {
        x -= width / 2;
    }

    for( const auto& te: list ) {
        al_draw_text( font, te.color, x, y, ALLEGRO_ALIGN_LEFT, te.str.c_str() );
        x += te.width;
    }

    return width;
}

std::string strip_color_text( const std::string& text )
{
    std::string result;
    bool color_mode = false;

    for( const auto& i : text )
    {
        if( i == '^' )
        {
            color_mode = ! color_mode;
        } else {
            if( !color_mode )
            {
                result.push_back( i );
            }
        }
    }

    return result;
}


void test_colored_text( ALLEGRO_FONT* font )
{
    ALLEGRO_COLOR color = al_map_rgb( 255, 255, 255 );
    draw_colored_text( font, color, 200, 10, ALLEGRO_ALIGN_LEFT, "Hello World" );
    draw_colored_text( font, color, 200, 30, ALLEGRO_ALIGN_LEFT, "Hello ^^ World" );
    draw_colored_text( font, color, 200, 50, ALLEGRO_ALIGN_CENTRE, "Hello ^red^World" );
    draw_colored_text( font, color, 200, 70, ALLEGRO_ALIGN_LEFT, "N^red^i^!^gel Atk^red^i^!^nson" );
    draw_colored_text( font, color, 200, 90, ALLEGRO_ALIGN_RIGHT, "Nigel ^#FF11FF^Atkinson" );

    return;
}

