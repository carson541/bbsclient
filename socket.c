#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "socket.h"

#define SERVER                "localhost"
#define PORT                  23

static char g_server_addr[16];
static int g_socket_fd;
static unsigned char g_buf[256];

void test_socket(void)
{
    struct hostent *p;
    struct sockaddr_in addr;
    int n, i;
    
    printf("<test_socket>\n");

    p = gethostbyname(SERVER);
    if(p == NULL) {
        printf("error\n");
        return;
    }

    if(p->h_addrtype == AF_INET) {
        strcpy(g_server_addr,
               inet_ntop(p->h_addrtype, *(p->h_addr_list),
                         g_server_addr, sizeof(g_server_addr)));
        printf("g_server_addr = %s\n", g_server_addr);
    }

    g_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, g_server_addr, &addr.sin_addr);
    addr.sin_port = htons(PORT);

    connect(g_socket_fd, (struct sockaddr *)&addr, sizeof(addr));

    n = read(g_socket_fd, g_buf, 16);
    // dump
    printf("read: %d\n", n);
    for(i = 0; i < n; i++) {
        if(i % 16 == 0 && i != 0) printf("\n");
        printf("%.2x ", (unsigned char)g_buf[i]);
    }
    printf("\n");

    close(g_socket_fd);
}

int socket_create(void)
{
    struct hostent *p;
    struct sockaddr_in addr;
    
    printf("<socket_create>\n");

    p = gethostbyname(SERVER);
    if(p == NULL) {
        printf("error\n");
        return -1;
    }

    if(p->h_addrtype == AF_INET) {
        strcpy(g_server_addr,
               inet_ntop(p->h_addrtype, *(p->h_addr_list),
                         g_server_addr, sizeof(g_server_addr)));
        printf("g_server_addr = %s\n", g_server_addr);
    } else {
        printf("error\n");
        return -1;
    }

    g_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, g_server_addr, &addr.sin_addr);
    addr.sin_port = htons(PORT);

    connect(g_socket_fd, (struct sockaddr *)&addr, sizeof(addr));

    return g_socket_fd;
}

void socket_close(void)
{
    close(g_socket_fd);
}

int socket_read(void *buf, int len)
{
    return read(g_socket_fd, buf, len);
}

void socket_writen(void *buf, int len)
{
    int n, total;

    total = len;
    while(total > 0) {
        n = write(g_socket_fd, buf, total);
        if(n < 0) break;
        total -= n;
    }
}
