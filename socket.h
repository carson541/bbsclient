#ifndef _SOCKET_H_
#define _SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

void test_socket(void);
int socket_create(void);
void socket_close(void);
int socket_read(void *buf, int len);
void socket_writen(void *buf, int len);
    
#ifdef __cplusplus
}
#endif

#endif /* _SOCKET_H_ */
