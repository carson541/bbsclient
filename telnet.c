#include <stdio.h>

#include "socket.h"
#include "telnet.h"

#define IAC_START             1
#define IAC_OPTION            2

#define OPTION_WILL           1
#define OPTION_WONT           2
#define OPTION_DO             3
#define OPTION_DONT           4

int iac;
int option;

void telnet_init(void)
{
    iac = 0;
}

/*
 * return 1 - echo process by calling procedure
 *        0 - eat by telnet
 */ 
int telnet_parse(unsigned char c)
{
    if(iac & IAC_START) {
        if(iac & IAC_OPTION) {
            switch(c) {
            case 1:
                if(option == OPTION_DO) {
                    // reply IAC, WONT, ECHO
                    unsigned char buf[3] = {255, 252, 1};
                    socket_writen(buf, 3);
                    iac = 0;
                } else  if(option == OPTION_WILL) {
                    // reply IAC, DO, ECHO
                    unsigned char buf[3] = {255, 253, 1};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 3: // suppress go ahead
                if(option == OPTION_WILL) {
                    // reply IAC, DO, SGA
                    unsigned char buf[3] = {255, 253, 3};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 5: // status
                if(option == OPTION_WILL) {
                    // reply IAC, DO, STATUS
                    unsigned char buf[3] = {255, 253, 5};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 24: // terminal type
                if(option == OPTION_DO) {
                    // reply IAC, WONT, TTYPE
                    unsigned char buf[3] = {255, 252, 24};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 31: // window size
                if(option == OPTION_DO) {
                    // reply IAC, WONT, NAWS
                    unsigned char buf[3] = {255, 252, 31};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 32: // terminal speed
                if(option == OPTION_DO) {
                    // reply IAC, WONT, TSPEED
                    unsigned char buf[3] = {255, 252, 32};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 33: // remote flow control
                if(option == OPTION_DO) {
                    // reply IAC, WONT, LFLOW
                    unsigned char buf[3] = {255, 252, 33};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 35: // X Display Location
                if(option == OPTION_DO) {
                    // reply IAC, WONT, XDISPLOC
                    unsigned char buf[3] = {255, 252, 35};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            case 39: // New - Environment variables
                if(option == OPTION_DO) {
                    // reply IAC, WONT, NEW_ENVIRON
                    unsigned char buf[3] = {255, 252, 39};
                    socket_writen(buf, 3);
                    iac = 0;
                } else {
                    printf("unknown option = %d\n", option);
                }
                break;
            default:
                printf("unknown IAC OPTION 0x%.2X\n", c);
                break;
            }
        } else {
            switch(c) {
            case 251: // WILL
                iac |= IAC_OPTION;
                option = OPTION_WILL;
                break;
            case 253: // DO
                iac |= IAC_OPTION;
                option = OPTION_DO;
                break;
            default:
                printf("unknown IAC 0x%.2X\n", c);
                break;
            }
        }
    } else {
        if(c == 0xff) {
            // clear iac buffer
            iac = IAC_START;
        } else { // normal
            return 1; // echo
        }
    }

    return 0; // eat
}
