package com.sunny.bbsclient;

import android.os.Bundle;
import android.app.Activity;
// import android.graphics.Color;
// import android.graphics.Paint;
// import android.util.Log;
import android.view.Menu;

public class BBSActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_bbs);

        // TermView tv = (TermView)this.findViewById(R.id.termView);

        // Paint mTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        // mTextPaint.setColor(Color.WHITE);
        // mTextPaint.setTextSize(16);
        // String s = "test";
        // String s2 = "中文";
        // String displayText = "Hello World!";
        // float textWidth = mTextPaint.measureText(displayText);
        // float w1 = mTextPaint.measureText(s);
        // float w2 = mTextPaint.measureText(s2);
        // Log.d("bbsclient", "textWidth = " + textWidth);
        // Log.d("bbsclient", "w1 = " + w1);
        // Log.d("bbsclient", "w2 = " + w2);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.bb, menu);
		return true;
	}
}
