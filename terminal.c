#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "screen.h"
#include "terminal.h"

#define ESC_BUF_SIZ   256
#define ESC_ARG_SIZ   16

#define CURSOR_DEFAULT        0
#define CURSOR_WRAPNEXT       1

#define ESC_START             1
#define ESC_CSI               2

struct t_cell cell[ROWS][COLS];
int dirty[ROWS];
int cursor_mode;
int cursor_fg, cursor_bg;
int cursor_x, cursor_y;
int cursor_state;

/* CSI Escape sequence structs */
/* ESC '[' [[ [<priv>] <arg> [;]] <mode>] */
typedef struct {
    char buf[ESC_BUF_SIZ]; /* raw string */
    int len;               /* raw string length */
    char priv;
    int arg[ESC_ARG_SIZ];
    int narg;              /* nb of args */
    char mode;
} CSIEscape;

static CSIEscape escseq;

int term_top, term_bottom;
int esc;

void csireset(void);
void csiparse(void);
void csihandle(void);
void csidump(void);

void tclearregion(int x1, int y1, int x2, int y2);
void tsetchar(char c);
void tmoveto(int x, int y);
void tnewline(int first_col);
void tscrollup(int orig, int n);
void tsetattr(int *attr, int l);

void treset(void)
{
    tclearregion(0, 0, COLS - 1, ROWS - 1);

    cursor_mode = ATTR_NULL;
    cursor_fg = DefaultFG, cursor_bg = DefaultBG;
    cursor_x = 0, cursor_y = 0;
    cursor_state = CURSOR_DEFAULT;

    term_top = 0, term_bottom = ROWS - 1;

    esc = 0;
}

void tputc(char c)
{
    if(esc & ESC_START) {
        if(esc & ESC_CSI) {
            escseq.buf[escseq.len++] = c;
            if((c >= 0x40 && c <= 0x7E) || escseq.len >= ESC_BUF_SIZ) {
                esc = 0;
                csiparse();
                csihandle();
            }
        } else {
            switch(c) {
            case '[':
                esc |= ESC_CSI;
                break;
            default:
                printf("unknown sequence ESC 0x%02X '%c'\n",
                       c, isprint(c)? c : '.');
                esc = 0;
                break;
            }
        }
    } else {
        switch(c) {
        case '\b': // BS
            tmoveto(cursor_x - 1, cursor_y);
            break;
        case '\r': // CR
            tmoveto(0, cursor_y);
            break;
        case '\n': // LF
            tnewline(0);
            break;
        case '\a': // BEL
            break;
        case '\033': // ESC
            csireset();
            esc = ESC_START;
            break;
        default:
            if(cursor_state & CURSOR_WRAPNEXT) {
                tnewline(1);
            }
            tsetchar(c);
            if(cursor_x + 1 < COLS) {
                tmoveto(cursor_x + 1, cursor_y);
            } else {
                cursor_state |= CURSOR_WRAPNEXT;
            }
            break;
        }
    }
}

void csireset(void)
{
    memset(&escseq, 0, sizeof(escseq));
}

void csiparse(void)
{
    /* int noarg = 1; */
    char *p = escseq.buf;

    escseq.narg = 0;
    if(*p == '?')
        escseq.priv = 1, p++;

    while(p < escseq.buf + escseq.len) {
        while(isdigit(*p)) {
            escseq.arg[escseq.narg] *= 10;
            escseq.arg[escseq.narg] += *p++ - '0'/*, noarg = 0 */;
        }
        if(*p == ';' && escseq.narg+1 < ESC_ARG_SIZ)
            escseq.narg++, p++;
        else {
            escseq.mode = *p;
            escseq.narg++;
            return;
        }
    }
}

void csihandle(void)
{
    switch(escseq.mode) {
    case 'H': /* CUP -- Move to <row> <col> */
        if(!escseq.arg[0]) escseq.arg[0] = 1;
        if(!escseq.arg[1]) escseq.arg[1] = 1;
        tmoveto(escseq.arg[1] - 1, escseq.arg[0] - 1);
        break;
    case 'I': /* font */
        switch(escseq.arg[0]) {
        case 0: // back to normal
            break;
        case 1: // SongTi
            break;
        case 2: // HeiTi
            break;
        case 3: // KaiTi
            break;
        default:
            break;
        }
        break;
    case 'J': /* ED -- Clear screen */
        switch(escseq.arg[0]) {
        case 0: /* below */
            tclearregion(cursor_x, cursor_y, COLS - 1, cursor_y);
            if(cursor_y < ROWS - 1)
                tclearregion(0, cursor_y + 1, COLS - 1, ROWS - 1);
            break;
        case 1: /* above */
            if(cursor_y > 1)
                tclearregion(0, 0, COLS - 1, cursor_y - 1);
            tclearregion(0, cursor_y, cursor_x, cursor_y);
            break;
        case 2: /* all */
            tclearregion(0, 0, COLS - 1, ROWS - 1);
            break;
        default:
            printf("unknown csi ");
            csidump();
            break;
        }
        break;
    case 'K': /* EL -- Clear line */
        switch(escseq.arg[0]) {
        case 0: /* right */
            tclearregion(cursor_x, cursor_y, COLS - 1, cursor_y);
            break;
        case 1: /* left */
            tclearregion(0, cursor_y, cursor_x, cursor_y);
            break;
        case 2: /* all */
            tclearregion(0, cursor_y, COLS - 1, cursor_y);
            break;
        }
        break;
    case 'm': /* SGR -- Terminal attribute (color) */
        tsetattr(escseq.arg, escseq.narg);
        break;
    default:
        printf("unknown csi ");
        csidump();
        break;
    }
}

void csidump(void)
{
    int i;
    printf("ESC[");
    for(i = 0; i < escseq.len; i++) {
        unsigned int c = escseq.buf[i] & 0xff;
        if(isprint(c)) putchar(c);
        else if(c == '\n') printf("(\\n)");
        else if(c == '\r') printf("(\\r)");
        else if(c == 0x1b) printf("(\\e)");
        else printf("(%02x)", c);
    }
    putchar('\n');
}

void tclearregion(int x1, int y1, int x2, int y2)
{
    int x, y, temp;

    if(x1 > x2)
        temp = x1, x1 = x2, x2 = temp;
    if(y1 > y2)
        temp = y1, y1 = y2, y2 = temp;

    if(x1 < 0) x1 = 0;
    if(x1 > COLS - 1) x1 = COLS - 1;
    if(y1 < 0) y1 = 0;
    if(y1 > ROWS - 1) y1 = ROWS - 1;

    if(x2 < 0) x2 = 0;
    if(x2 > COLS - 1) x2 = COLS - 1;
    if(y2 < 0) y2 = 0;
    if(y2 > ROWS - 1) y2 = ROWS - 1;

    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            cell[y][x].state = 0;
        }
        dirty[y] = 1;
    }
}

void tsetchar(char c)
{
    cell[cursor_y][cursor_x].mode = cursor_mode;
    cell[cursor_y][cursor_x].fg = cursor_fg;
    cell[cursor_y][cursor_x].bg = cursor_bg;

    cell[cursor_y][cursor_x].c = c;

    cell[cursor_y][cursor_x].state |= GLYPH_SET;

    dirty[cursor_y] = 1;
}

void tmoveto(int x, int y)
{
    if(x < 0) x = 0;
    if(x > COLS - 1) x = COLS - 1;
    if(y < 0) y = 0;
    if(y > ROWS - 1) y = ROWS - 1;

    cursor_x = x;
    cursor_y = y;
    cursor_state &= ~CURSOR_WRAPNEXT;
}

void tnewline(int first_col)
{
    int y = cursor_y;
    if(y == term_bottom) {
        tscrollup(term_top, 1);
    } else {
        y++;
    }

    tmoveto(first_col ? 0 : cursor_x, y);
}

void tscrollup(int orig, int n)
{
    int i, j;
    struct t_cell temp;

    if(n < 0) n = 0;
    if(n > term_bottom - orig + 1) n = term_bottom - orig + 1;

    tclearregion(0, orig, COLS - 1, orig + n - 1);

    for(i = orig; i <= term_bottom - n; i++) {
        for(j = 0; j < COLS; j++) {
            temp = cell[i][j];
            cell[i][j] = cell[i+n][j];
            cell[i+n][j] = temp;
        }

        dirty[i] = 1;
        dirty[i+n] = 1;
    }
}

void tsetattr(int *attr, int l)
{
    int i;

    for(i = 0; i < l; i++) {
        switch(attr[i]) {
        case 0:
            cursor_mode &= ~(ATTR_REVERSE | ATTR_UNDERLINE | ATTR_BOLD);
            cursor_fg = DefaultFG;
            cursor_bg = DefaultBG;
            break;
        case 1:
            cursor_mode |= ATTR_BOLD;
            break;
        case 5:
            cursor_mode |= ATTR_BLINK;
            break;
        case 7:
            cursor_mode |= ATTR_REVERSE;
            break;
        default:
            if(attr[i] >= 30 && attr[i] <= 37) {
                cursor_fg = attr[i] - 30;
            } else if(attr[i] >= 40 && attr[i] <= 47) {
                cursor_bg = attr[i] - 40;
            } else {
                printf("erresc: gfx attr %d unknown\n", attr[i]), csidump();
            }
            break;
        }
    }
}
