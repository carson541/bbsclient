#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define COLS 80
#define ROWS 24

#define GLYPH_SET             1

#define ATTR_NULL             0
#define ATTR_REVERSE          1
#define ATTR_UNDERLINE        2
#define ATTR_BOLD             4
#define ATTR_BLINK            8

struct t_cell {
    char c;
    int mode;
    int fg;
    int bg;
    int state;
};

extern struct t_cell cell[ROWS][COLS];

extern int cursor_fg, cursor_bg;

void treset(void);
void tputc(char c);
    
#ifdef __cplusplus
}
#endif

#endif /* _TERMINAL_H_ */
