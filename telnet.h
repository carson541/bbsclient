#ifndef _TELNET_H_
#define _TELNET_H_

#ifdef __cplusplus
extern "C" {
#endif

void telnet_init(void);
int telnet_parse(unsigned char c);

#ifdef __cplusplus
}
#endif

#endif /* _TELNET_H_ */
