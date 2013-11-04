#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

// void func(void *);
void new_thread(void *func, void *arg);
void exit_threadfunc(void);
    
#ifdef __cplusplus
}
#endif

#endif /* _THREAD_H_ */
