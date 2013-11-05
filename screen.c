#include <stdio.h>

#include "terminal.h"
#include "ui_callback.h"

#include "screen.h"

void screen_redraw(void *context, void *drawable)
{
    int x, y;
    int ic, ib, ox;
    int base_mode, new_mode;
    int base_fg, new_fg;
    int base_bg, new_bg;
    int new_state;
    char str[256];

//    test_ui_callback();
    
    for(y = 0; y < ROWS; y++) {
        xclear(context, drawable, 0, y, COLS, y);

        ic = ib = ox = 0;

        for(x = 0; x < COLS; x++) {
            new_state = cell[y][x].state;
            new_mode = cell[y][x].mode;
            new_fg = cell[y][x].fg;
            new_bg = cell[y][x].bg;

            if(ib > 0 && (!(new_state & GLYPH_SET) ||
                          (base_mode != new_mode) ||
                          (base_fg != new_fg) ||
                          (base_bg != new_bg))) {
                xdraws(context, drawable,
                       base_mode, base_fg, base_bg,
                       str, ox, y, ic, ib);
                ic = ib = 0;
            }

            if(new_state & GLYPH_SET) {
                if(ib == 0) {
                    ox = x;
                    base_mode = new_mode;
                    base_fg = new_fg;
                    base_bg = new_bg;
                }
                str[ib++] = cell[y][x].c;
                ic ++;
            }
        }
        if(ib > 0) {
            xdraws(context, drawable,
                   base_mode, base_fg, base_bg,
                   str, ox, y, ic, ib);
        }
    }

    xdrawcursor(context, drawable, cursor_x, cursor_y);
}
