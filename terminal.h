#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define COLS 80
#define ROWS 24

void treset(void);
void tputc(char c);
    
#ifdef __cplusplus
}
#endif

#endif /* _TERMINAL_H_ */
