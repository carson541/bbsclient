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
	public TermView(Context context) {
		super(context);
	}

	public TermView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public TermView(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
	}

	// @Override
	// protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
	// 	super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	// }

    @Override
	protected void onDraw(Canvas canvas) {
        int width = getMeasuredWidth();
        int height = getMeasuredHeight();
        Log.d("bbsclient", "width = " + width);
        Log.d("bbsclient", "height = " + height);
        Paint mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        // Typeface font = Typeface.create(Typeface.MONOSPACE,
        //                                 Typeface.NORMAL);
        Typeface font = Typeface.createFromAsset(getResources().getAssets(),
                                                 "UbuntuMono-R.ttf");
        mTextPaint.setColor(Color.BLACK);
        mTextPaint.setTypeface(font);
        mTextPaint.setTextSize(20);
        String s1 = "test";
        String s2 = "中文";
        float w1 = mTextPaint.measureText(s1);
        float w2 = mTextPaint.measureText(s2);
        Log.d("bbsclient", "w1 = " + w1);
        Log.d("bbsclient", "w2 = " + w2);
        canvas.drawText(s1, 10, 50, mTextPaint);
        canvas.drawText(s2, 10, 80, mTextPaint);
	}
}
