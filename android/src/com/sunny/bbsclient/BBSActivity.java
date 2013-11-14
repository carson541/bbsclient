package com.sunny.bbsclient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class BBSActivity extends Activity {
    BBSThread mThread;
    Socket mSocket;
    private String server = "bbs.sjtu.edu.cn";
    private static final int PORT = 23;
    DataInputStream in;
    DataOutputStream out;
    boolean bIsConnected;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bbs);

        bIsConnected = false;

        TermView tv = (TermView)this.findViewById(R.id.termView);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.bb, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case (R.id.action_connect):
            Log.d("bbsclient", "connect");
            if(!bIsConnected) {
                bIsConnected = true;
                mThread = new BBSThread();
                mThread.start();
            }
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
                Log.d("bbsclient", "read: " + n);

                // try {
                //     Thread.sleep(1000);
                // } catch (InterruptedException e) {
                //  e.printStackTrace();
                // }
            }

        }
    }
}
