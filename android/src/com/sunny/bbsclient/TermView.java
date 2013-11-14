package com.sunny.bbsclient;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

public class TermView extends View {
    private Paint mTextPaint;
    private Typeface mFont;
    private int cell_width;
    private int cell_height;

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
        // float w1 = mTextPaint.measureText(s1);
        // float w2 = mTextPaint.measureText(s2);
        // Log.d("bbsclient", "w1 = " + w1);
        // Log.d("bbsclient", "w2 = " + w2);
        // canvas.drawText(s1, 10, 50, mTextPaint);
        // canvas.drawText(s2, 10, 80, mTextPaint);
    }

    private void init() {
        mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mFont = Typeface.createFromAsset(getResources().getAssets(),
                                         "UbuntuMono-R.ttf");
        mTextPaint.setColor(Color.BLACK);
        mTextPaint.setTypeface(mFont);
        mTextPaint.setTextSize(16);

        cell_width = 16;
        cell_height = 16;
    }
}
