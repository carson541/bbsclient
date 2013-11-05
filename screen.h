#ifndef _SCREEN_H_
#define _SCREEN_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DefaultFG             7
#define DefaultBG             0

void screen_redraw(void *context, void *drawable);
    
#ifdef __cplusplus
}
#endif

#endif /* _SCREEN_H_ */
