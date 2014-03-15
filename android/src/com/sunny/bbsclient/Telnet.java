package com.sunny.bbsclient;

import java.io.DataOutputStream;
import java.io.IOException;

import android.util.Log;

public class Telnet {
    private static final int IAC_START = 1;
    private static final int IAC_OPTION = 2;

    private static final int OPTION_WILL = 1;
    private static final int OPTION_WONT = 2;
    private static final int OPTION_DO = 3;
    private static final int OPTION_DONT = 4;

    private int iac;
    private int option;

    private DataOutputStream mOut;

    Telnet() {
    }

    public void init() {
        iac = 0;
    }

   /*
    * return true  - echo process by calling procedure
    *        false - eat by telnet
    */
    public boolean parse(byte c) {
        if((iac & IAC_START) != 0) {
            if((iac & IAC_OPTION) != 0) {
                switch(c) {
                case 1:
                    if(option == OPTION_DO) {
                        // reply IAC, WONT, ECHO
                        byte[] buf = {(byte)255, (byte)252, (byte)1};
                        writen(buf, 3);
                        iac = 0;
                    } else  if(option == OPTION_WILL) {
                        // reply IAC, DO, ECHO
                        byte[] buf = {(byte)255, (byte)253, (byte)1};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 3: // suppress go ahead
                    if(option == OPTION_WILL) {
                        // reply IAC, DO, SGA
                        byte[] buf = {(byte)255, (byte)253, (byte)3};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 5: // status
                    if(option == OPTION_WILL) {
                        // reply IAC, DO, STATUS
                        byte[] buf = {(byte)255, (byte)253, (byte)5};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 24: // terminal type
                    if(option == OPTION_DO) {
                        // reply IAC, WONT, TTYPE
                        byte[] buf = {(byte)255, (byte)252, (byte)24};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 31: // window size
                    if(option == OPTION_DO) {
                        // reply IAC, WONT, NAWS
                        byte[] buf = {(byte)255, (byte)252, (byte)31};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 32: // terminal speed
                    if(option == OPTION_DO) {
                        // reply IAC, WONT, TSPEED
                        byte[] buf = {(byte)255, (byte)252, (byte)32};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 33: // remote flow control
                    if(option == OPTION_DO) {
                        // reply IAC, WONT, LFLOW
                        byte[] buf = {(byte)255, (byte)252, (byte)33};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 35: // X Display Location
                    if(option == OPTION_DO) {
                        // reply IAC, WONT, XDISPLOC
                        byte[] buf = {(byte)255, (byte)252, (byte)35};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                case 39: // New - Environment variables
                    if(option == OPTION_DO) {
                        // reply IAC, WONT, NEW_ENVIRON
                        byte[] buf = {(byte)255, (byte)252, (byte)39};
                        writen(buf, 3);
                        iac = 0;
                    } else {
                        Log.d("bbsclient", "unknown option " + option);
                    }
                    break;
                default:
                    Log.d("bbsclient", "unknown IAC OPTION " + c);
                    break;
                }
            } else {
                switch(c) {
                case (byte)(251 - 256): // WILL
                    iac |= IAC_OPTION;
                    option = OPTION_WILL;
                    break;
                case (byte)(253 - 256): // DO
                    iac |= IAC_OPTION;
                    option = OPTION_DO;
                    break;
                default:
                    Log.d("bbsclient", "unknown IAC " + c);
                    break;
                }
            }
        } else {
            if(c == (byte)(255 - 256)) {
                // clear iac buffer
                iac = IAC_START;
            } else { // normal
                return true; // echo
            }
        }

        return false; // eat
    }

    private void writen(byte buf[], int n) {
        try {
            mOut.write(buf, 0, n);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void setDataOutputStream(DataOutputStream out) {
        mOut = out;
    }
}
