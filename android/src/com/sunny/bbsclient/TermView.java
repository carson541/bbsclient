package com.sunny.bbsclient;

import java.io.UnsupportedEncodingException;
import java.util.Arrays;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

public class TermView extends View {
    private Paint mPaint;
    private Typeface mFont;
    private int cell_width;
    private int cell_height;
    Terminal mTerminal;
    int palette_xterm[];

    public TermView(Context context) {
        super(context);
        init();
    }

    public TermView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public TermView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    // @Override
    // protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    //  super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    // }

    @Override
    protected void onDraw(Canvas canvas) {
        // int width = getMeasuredWidth();
        // int height = getMeasuredHeight();
        // Log.d("bbsclient", "width = " + width);
        // Log.d("bbsclient", "height = " + height);
        // String s1 = "test";
        // String s2 = "中文";
        // byte str[] = new byte[12];
        // str[0] = str[8] = 0x74;
        // str[1] = str[9] = 0x65;
        // str[2] = str[10] = 0x73;
        // str[3] = str[11] = 0x74;
        // str[4] = (byte) 0xd6;
        // str[5] = (byte) 0xd0;
        // str[6] = (byte) 0xce;
        // str[7] = (byte) 0xc4;
        // Log.d("bbsclient", "str = " + Arrays.toString(str));
        // String s3_gbk = null;
        // try {
        //  s3_gbk = new String(str, 0, 12, "GBK");
        // } catch (UnsupportedEncodingException e1) {
        //  e1.printStackTrace();
        // }
        // String s3 = null;
        // try {
        //  s3 = new String(s3_gbk.getBytes("GBK"), "UTF-8");
        // } catch (UnsupportedEncodingException e) {
        //  e.printStackTrace();
        // }
        // String s4 = "test中文test";
        // String s4_gbk = null;
        // byte[] b_gbk = null;
        // try {
        //     s4_gbk = new String(s4.getBytes("UTF-8"), "GBK");
        //     b_gbk = s4.getBytes("GBK");
        // } catch (UnsupportedEncodingException e) {
        //  e.printStackTrace();
        // }
        // int[] b_gbk = new int[s4_gbk.length()];
        // for(int i = 0; i < s4_gbk.length(); i++) {
        //     b_gbk[i] = (int)s4_gbk.charAt(i);
        // }
        // Log.d("bbsclient", "b_gbk = " + Arrays.toString(b_gbk));
        // float w1 = mPaint.measureText(s1);
        // float w2 = mPaint.measureText(s2);
        // Log.d("bbsclient", "w1 = " + w1);
        // Log.d("bbsclient", "w2 = " + w2);
        // Log.d("bbsclient", "s1 = " + s1);
        // Log.d("bbsclient", "s2 = " + s2);
        // Log.d("bbsclient", "s3_gbk = " + s3_gbk);
        // Log.d("bbsclient", "s3 = " + s3);
        // canvas.drawText(s1, 10, 50, mPaint);
        // canvas.drawText(s2, 10, 80, mPaint);
        // canvas.drawText(s3_gbk, 10, 110, mPaint);

        screen_redraw(canvas);
    }

    private void init() {
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mFont = Typeface.createFromAsset(getResources().getAssets(),
                                         "UbuntuMono-R.ttf");
        mPaint.setColor(Color.BLACK);
        mPaint.setTypeface(mFont);
        mPaint.setTextSize(16);

        cell_width = 16;
        cell_height = 16;

        palette_xterm = new int[16];
        palette_xterm[0] = Color.argb(0xff, 0x00, 0x00, 0x00); /* black */
        palette_xterm[1] = Color.argb(0xff, 0xcd, 0x00, 0x00); /* red3 */
        palette_xterm[2] = Color.argb(0xff, 0x00, 0xcd, 0x00); /* green3 */
        palette_xterm[3] = Color.argb(0xff, 0xcd, 0xcd, 0x00); /* yellow3 */
        palette_xterm[4] = Color.argb(0xff, 0x00, 0x00, 0xee); /* blue2 */
        palette_xterm[5] = Color.argb(0xff, 0xcd, 0x00, 0xcd); /* magenta3 */
        palette_xterm[5] = Color.argb(0xff, 0x00, 0xcd, 0xcd); /* cyan3 */
        palette_xterm[7] = Color.argb(0xff, 0xe5, 0xe5, 0xe5); /* gray90 */
        palette_xterm[8] = Color.argb(0xff, 0x7f, 0x7f, 0x7f); /* gray50 */
        palette_xterm[9] = Color.argb(0xff, 0xff, 0x00, 0x00); /* red */
        palette_xterm[10] = Color.argb(0xff, 0x00, 0xff, 0x00); /* green */
        palette_xterm[11] = Color.argb(0xff, 0xff, 0xff, 0x00); /* yellow */
        palette_xterm[12] = Color.argb(0xff, 0x5c, 0x5c, 0xff);
        palette_xterm[13] = Color.argb(0xff, 0xff, 0x00, 0xff); /* magenta */
        palette_xterm[14] = Color.argb(0xff, 0x00, 0xff, 0xff); /* cyan */
        palette_xterm[15] = Color.argb(0xff, 0xff, 0xff, 0xff); /* white */
    }

    public void setTerminal(Terminal t) {
        mTerminal = t;
    }

    private void screen_redraw(Canvas canvas) {
        int x, y;
        int ic, ib, ox;
        int base_mode = 0, new_mode;
        int base_fg = 0, new_fg;
        int base_bg = 0, new_bg;
        int new_state;
        char str[] = new char[256];

        for(y = 0; y < Terminal.ROWS; y++) {
            xclear(canvas, 0, y, Terminal.COLS, y);

            ic = ib = ox = 0;

            for(x = 0; x < mTerminal.COLS; x++) {
                new_state = mTerminal.cell[y][x].state;
                new_mode = mTerminal.cell[y][x].mode;
                new_fg = mTerminal.cell[y][x].fg;
                new_bg = mTerminal.cell[y][x].bg;

                if(ib > 0 && (((new_state & mTerminal.GLYPH_SET) == 0) ||
                              (base_mode != new_mode) ||
                              (base_fg != new_fg) ||
                              (base_bg != new_bg))) {
                    xdraws(canvas,
                           base_mode, base_fg, base_bg,
                           str, ox, y, ic, ib);
                    ic = ib = 0;
                }

                if((new_state & mTerminal.GLYPH_SET) != 0) {
                    if(ib == 0) {
                        ox = x;
                        base_mode = new_mode;
                        base_fg = new_fg;
                        base_bg = new_bg;
                    }
                    str[ib++] = mTerminal.cell[y][x].c;
                    ic ++;
                }
            }
            if(ib > 0) {
                xdraws(canvas,
                       base_mode, base_fg, base_bg,
                       str, ox, y, ic, ib);
            }
        }
    }

    private void xclear(Canvas canvas,
                        int x1, int y1, int x2, int y2) {
        // Log.d("bbsclient", "xclear, y1 = " + y1);
        Rect rect = new Rect(x1 * cell_width,
                             y1 * cell_height,
                             cell_width * x2 + 1,
                             cell_height * y2 + 1);
        // Log.d("bbsclient", "xclear, rect = " + rect);
        mPaint.setColor(palette_xterm[mTerminal.cursor_bg]);
        canvas.drawRect(rect, mPaint);
        mPaint.setColor(palette_xterm[mTerminal.cursor_fg]);
    }

    private void xdraws(Canvas canvas,
                        int mode, int fg, int bg,
                        char[] str, int x, int y, int charlen, int bytelen) {
        int temp;

        if((mode & mTerminal.ATTR_REVERSE) != 0) {
            temp = fg; fg = bg; bg = temp;
        }

        if((mode & mTerminal.ATTR_BOLD) != 0) {
            if(fg < 8) {
                fg += 8;
            }
        }

        Rect rect = new Rect(x * cell_width,
                             y * cell_height,
                             cell_width * (x + bytelen) + 1,
                             cell_height * (y + 1) + 1);
        mPaint.setColor(palette_xterm[bg]);
        canvas.drawRect(rect, mPaint);

        int color = palette_xterm[fg];
        mPaint.setColor(color);

        str[charlen] = '\0';
        byte[] btr = new byte[charlen];
        for(int i = 0; i < charlen; i++) {
            btr[i] = (byte)str[i];
        }

        String s = null;
        try {
            s = new String(btr, 0, charlen, "GBK");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        Log.d("bbsclient", "s = " + s);
        canvas.drawText(s,
                        x * cell_width,
                        y * cell_height,
                        mPaint);
    }
}
