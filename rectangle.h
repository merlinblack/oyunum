#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <allegro5/allegro_primitives.h>
#include <memory>
#include "renderlist.h"

class Rectangle : public Renderable
{
    ALLEGRO_COLOR color;
    int x1; int y1;
    int x2; int y2;
    int rx; int ry;

    public:

    Rectangle( int px1, int py1, int px2, int py2, int prx, int pry, ALLEGRO_COLOR pcolor )
        : x1(px1), y1(py1), x2(px2), y2(py2)
        , rx(prx), ry(pry)
        , color(pcolor)
    {}

    void render()
    {
        if( rx == 0 && ry == 0 ) {
            al_draw_filled_rectangle( x1, y1, x2, y2, color );
        } else {
            al_draw_filled_rounded_rectangle( x1, y1, x2, y2, rx, ry, color );
        }
    }

};

using RectanglePtr = std::shared_ptr<Rectangle>;

#endif // RECTANGLE_H

