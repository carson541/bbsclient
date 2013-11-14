package com.sunny.bbsclient;

import android.util.Log;

public class Terminal {
    public static final int COLS = 80;
    public static final int ROWS = 24;

    public static final int GLYPH_SET = 1;

    public static final int ATTR_NULL = 0;
    public static final int ATTR_REVERSE = 1;
    public static final int ATTR_UNDERLINE = 2;
    public static final int ATTR_BOLD = 4;
    public static final int ATTR_BLINK = 8;

    public static final int DefaultFG = 7;
    public static final int DefaultBG = 0;

    public Cell[][] cell;

    public int cursor_fg, cursor_bg;
    public int cursor_x, cursor_y;

    private static final int ESC_BUF_SIZ = 256;
    private static final int ESC_ARG_SIZ = 16;

    private static final int CURSOR_DEFAULT = 0;
    private static final int CURSOR_WRAPNEXT = 1;

    private static final int ESC_START = 1;
    private static final int ESC_CSI = 2;

    private boolean[] dirty;

    private int cursor_mode;
    private int cursor_state;

    CSIEscape escseq;

    private int term_top, term_bottom;
    private int esc;

    /* CSI Escape sequence structs */
    /* ESC '[' [[ [<priv>] <arg> [;]] <mode>] */
    class CSIEscape {
        char[] buf = new char[ESC_BUF_SIZ]; /* raw string */
        int len;               /* raw string length */
        char priv;
        int[] arg = new int[ESC_ARG_SIZ];
        int narg;              /* nb of args */
        char mode;
    };

    Terminal() {
        init();
    }

    private void init() {
        cell = new Cell[ROWS][COLS];
        for(int i = 0; i < ROWS; i++) {
            for(int j = 0; j < COLS; j++) {
                cell[i][j] = new Cell();
            }
        }
        dirty = new boolean[ROWS];
        escseq = new CSIEscape();
    }

    public void reset() {
        clearregion(0, 0, COLS - 1, ROWS - 1);

        cursor_mode = ATTR_NULL;
        cursor_fg = DefaultFG; cursor_bg = DefaultBG;
        cursor_x = 0; cursor_y = 0;
        cursor_state = CURSOR_DEFAULT;

        term_top = 0; term_bottom = ROWS - 1;

        esc = 0;
    }

    public void putc(char c) {
        if((esc & ESC_START) != 0) {
            if((esc & ESC_CSI) != 0) {
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
                    Log.d("bbsclient",
                          "unknown sequence ESC 0x%02X '" +
                          (isprint(c)? c : '.') + "'");
                    esc = 0;
                    break;
                }
            }
        } else {
            switch(c) {
            case '\b': // BS
                moveto(cursor_x - 1, cursor_y);
                break;
            case '\r': // CR
                moveto(0, cursor_y);
                break;
            case '\n': // LF
                newline(false);
                break;
            case '\07': // BEL
                break;
            case '\033': // ESC
                csireset();
                esc = ESC_START;
                break;
            default:
                if((cursor_state & CURSOR_WRAPNEXT) != 0) {
                    newline(true);
                }
                setchar(c);
                if(cursor_x + 1 < COLS) {
                    moveto(cursor_x + 1, cursor_y);
                } else {
                    cursor_state |= CURSOR_WRAPNEXT;
                }
                break;
            }
        }
    }

    private void csireset() {
        escseq = new CSIEscape();
    }

    private void csiparse() {
        /* int noarg = 1; */
        char[] p = escseq.buf;
        int i = 0;

        escseq.narg = 0;
        if(p[i] == '?') {
            escseq.priv = 1; i++;
        }

        while(i < escseq.len) {
            while(isdigit(p[i])) {
                escseq.arg[escseq.narg] *= 10;
                escseq.arg[escseq.narg] += p[i++] - '0'/*, noarg = 0 */;
            }
            if(p[i] == ';' && escseq.narg+1 < ESC_ARG_SIZ) {
                escseq.narg++; i++;
            } else {
                escseq.mode = p[i];
                escseq.narg++;
                return;
            }
        }
    }

    private void csihandle() {
        switch(escseq.mode) {
        case 'H': /* CUP -- Move to <row> <col> */
            if(escseq.arg[0] == 0) escseq.arg[0] = 1;
            if(escseq.arg[1] == 0) escseq.arg[1] = 1;
            moveto(escseq.arg[1] - 1, escseq.arg[0] - 1);
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
                clearregion(cursor_x, cursor_y, COLS - 1, cursor_y);
                if(cursor_y < ROWS - 1)
                    clearregion(0, cursor_y + 1, COLS - 1, ROWS - 1);
                break;
            case 1: /* above */
                if(cursor_y > 1)
                    clearregion(0, 0, COLS - 1, cursor_y - 1);
                clearregion(0, cursor_y, cursor_x, cursor_y);
                break;
            case 2: /* all */
                clearregion(0, 0, COLS - 1, ROWS - 1);
                break;
            default:
                Log.d("bbsclient", "unknown csi " + csidump());
                break;
            }
            break;
        case 'K': /* EL -- Clear line */
            switch(escseq.arg[0]) {
            case 0: /* right */
                clearregion(cursor_x, cursor_y, COLS - 1, cursor_y);
                break;
            case 1: /* left */
                clearregion(0, cursor_y, cursor_x, cursor_y);
                break;
            case 2: /* all */
                clearregion(0, cursor_y, COLS - 1, cursor_y);
                break;
            }
            break;
        case 'm': /* SGR -- Terminal attribute (color) */
            setattr(escseq.arg, escseq.narg);
            break;
        default:
            Log.d("bbsclient", "unknown csi " + csidump());
            break;
        }
    }

    String csidump() {
        StringBuilder sb = new StringBuilder();
        int i;
        sb.append("ESC[");
        for(i = 0; i < escseq.len; i++) {
            int c = escseq.buf[i] & 0xff;
            if(isprint((char)c)) sb.append((char)c);
            else if(c == '\n') sb.append("(\\n)");
            else if(c == '\r') sb.append("(\\r)");
            else if(c == 0x1b) sb.append("(\\e)");
            else sb.append(Integer.toString(c, 16));
        }

        return sb.toString();
    }

    private void clearregion(int x1, int y1, int x2, int y2) {
        int x, y, temp;

        if(x1 > x2) {
            temp = x1; x1 = x2; x2 = temp;
        }
        if(y1 > y2) {
            temp = y1; y1 = y2; y2 = temp;
        }

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
            dirty[y] = true;
        }
    }

    private void setchar(char c) {
        cell[cursor_y][cursor_x].mode = cursor_mode;
        cell[cursor_y][cursor_x].fg = cursor_fg;
        cell[cursor_y][cursor_x].bg = cursor_bg;

        cell[cursor_y][cursor_x].c = c;

        cell[cursor_y][cursor_x].state |= GLYPH_SET;

        dirty[cursor_y] = true;
    }

    private void moveto(int x, int y) {
        if(x < 0) x = 0;
        if(x > COLS - 1) x = COLS - 1;
        if(y < 0) y = 0;
        if(y > ROWS - 1) y = ROWS - 1;

        cursor_x = x;
        cursor_y = y;
        cursor_state &= ~CURSOR_WRAPNEXT;
    }

    private void newline(boolean first_col) {
        int y = cursor_y;
        if(y == term_bottom) {
            scrollup(term_top, 1);
        } else {
            y++;
        }

        moveto(first_col ? 0 : cursor_x, y);
    }

    private void scrollup(int orig, int n) {
        int i, j;
        Cell temp;

        if(n < 0) n = 0;
        if(n > term_bottom - orig + 1) n = term_bottom - orig + 1;

        clearregion(0, orig, COLS - 1, orig + n - 1);

        for(i = orig; i <= term_bottom - n; i++) {
            for(j = 0; j < COLS; j++) {
                temp = cell[i][j];
                cell[i][j] = cell[i+n][j];
                cell[i+n][j] = temp;
            }

            dirty[i] = true;
            dirty[i+n] = true;
        }
    }

    private void setattr(int[] attr, int l) {
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
                    Log.d("bbsclient", "erresc: gfx attr " +
                          attr[i] + " unknown");
                    Log.d("bbsclient", "" + csidump());
                }
                break;
            }
        }
    }

    private boolean isdigit(char c) {
        return (c >= '0' && c <= '9');
    }

    private boolean isprint(char c) {
        if(c >= '0' && c <= '9') return true;
        if(c >= 'A' && c <= 'Z') return true;
        if(c >= 'a' && c <= 'z') return true;
        return false;
    }
}
