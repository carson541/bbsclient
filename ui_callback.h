#ifndef _UI_CALLBACK_H_
#define _UI_CALLBACK_H_

void test_ui_callback(void);
void xclear(void *context, void *drawable,
            int x1, int y1, int x2, int y2);
void xdraws(void *context, void *drawable,
            int mode, int fg, int bg,
            char *str, int x, int y, int charlen, int bytelen);
void xdrawcursor(void *context, void *drawable,
                 int cursor_x, int cursor_y);

#endif /* _UI_CALLBACK_H_ */
