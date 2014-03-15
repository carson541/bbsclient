package com.sunny.bbsclient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class BBSActivity extends Activity {
    private BBSThread mThread;
    private Socket mSocket;
    private String server = "bbs.sjtu.edu.cn";
    private static final int PORT = 23;
    private DataInputStream in;
    private DataOutputStream out;
    private boolean bIsConnected;
    private Terminal mTerminal;
    private Telnet mTelnet;
    private TermView mTermView;
    private Timer mTimer = null;
    private TimerTask mTimerTask = null;
    private Handler mHandler = null;
    private static final int UPDATE_VIEW = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bbs);

        Log.d("bbsclient", "onCreate");

        // setRequestedOrientation(
        //     ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        mTermView = (TermView)this.findViewById(R.id.termView);

        mTerminal = new Terminal();
        mTerminal.reset();

        mTermView.setTerminal(mTerminal);
        mTermView.setActivity(this);

        mTelnet = new Telnet();
        mTelnet.init();

        bIsConnected = false;

        mHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                switch(msg.what) {
                case UPDATE_VIEW:
                    // Log.d("bbsclient", "UPDATE_VIEW");
                    mTermView.invalidate();
                    break;
                default:
                    break;
                }
            }
        };
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.bb, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.action_connect:
            Log.d("bbsclient", "connect");
            if(!bIsConnected) {
                bIsConnected = true;
                startTimer();
                mThread = new BBSThread();
                mThread.start();
            }
            return true;
        case R.id.action_disconnect:
            if(bIsConnected) {
                try {
                    mSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            Log.d("bbsclient", "disconnect");
            return true;
        case R.id.toggle_input:
            // (new Timer()).schedule(new TimerTask() {
            //         @Override public void run() {
            //             Log.d("bbsclient", "toggle input task");
            //             mTermView.toggleInput();
            //         }
            //     }, 2000);
            Log.d("bbsclient", "toggle input");
            return true;
        default:
            return false;
        }
    }

    class BBSThread extends Thread {
        public void run() {
            try {
                mSocket = new Socket(server, PORT);
                in = new DataInputStream(mSocket.getInputStream());
                out = new DataOutputStream(mSocket.getOutputStream());
                mTelnet.setDataOutputStream(out);
            } catch (UnknownHostException e1) {
                e1.printStackTrace();
            } catch (IOException e1) {
                e1.printStackTrace();
            }

            while(bIsConnected) {
                byte buf[] = new byte[16];
                int n = 0;
                try {
                    n = in.read(buf, 0, 16);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                // Log.d("bbsclient", "read: " + n);
                // Log.d("bbsclient", Arrays.toString(buf));

                for(int i = 0; i < n; i++) {
                    boolean echo = mTelnet.parse(buf[i]);
                    // Log.d("bbsclient", "echo: " + echo);
                    if(echo) mTerminal.putc((char)buf[i]);
                }

                if(n <= 0) break;
            }

            try {
                in.close();
                out.close();
                mSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            bIsConnected = false;
            stopTimer();
            mTerminal.reset();
            Message msg = Message.obtain(mHandler, UPDATE_VIEW);
            mHandler.sendMessage(msg);
        }
    }

    private void startTimer() {
        mTimer = new Timer();
        mTimerTask = new TimerTask() {
            @Override
            public void run () {
                // Log.d("bbsclient", "TimerTask");
                Message msg = Message.obtain(mHandler, UPDATE_VIEW);
                mHandler.sendMessage(msg);
            }
        };
        mTimer.schedule(mTimerTask, 1000, 1000); // delay, period
    }

    private void stopTimer() {
        mTimer.cancel();
        mTimerTask.cancel();
    }
}
