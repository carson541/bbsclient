#include <stdio.h>
#include <pthread.h>

#include "thread.h"

void new_thread(void *func, void *arg)
{
	int retcode;
	pthread_t th;

    printf("<new_thread>\n");

    retcode = pthread_create(&th, NULL,
		(void *)func, arg);
	if(retcode != 0) {
        printf("error\n");
    }
}

/*
 * called by thread 
 */ 
void exit_threadfunc(void)
{
    printf("<exit_threadfunc>\n");
    
    pthread_exit(NULL);
}
