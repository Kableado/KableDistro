#ifdef _WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>

#define THR_TYPE DWORD WINAPI

static void *__thread_create(DWORD (WINAPI *func)(PVOID),void *arg)
{ return (void *)CreateThread(NULL,64*1024,func,arg,0,NULL); }
static void  __thread_yield(void)	{ (void)SwitchToThread(); }
static int   __thread_wait(void *h)
{ DWORD ret;
    if (WaitForSingleObject(h,INFINITE)==WAIT_OBJECT_0 &&
	GetExitCodeThread(h,&ret))
    {	CloseHandle(h);
	return ret;
    }
  return -1;
}
static void *__semaphore_create(int max){ return CreateSemaphore(NULL,0,max,NULL); }
static int   __semaphore_wait(void *h)	{ return WaitForSingleObject(h,INFINITE)!=WAIT_FAILED; }
static int   __semaphore_post(void *h)	{ return ReleaseSemaphore(h,1,NULL); }
static void  __semaphore_close(void *h)	{ CloseHandle(h); }

#else

#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdlib.h>

#define THR_TYPE int

static void *__thread_create(int (*func)(void *),void *arg)
{ pthread_t h;
  pthread_attr_t attr;

    if (pthread_attr_init(&attr)==0 &&
	pthread_attr_setstacksize(&attr,64*1024)==0 &&
	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM)==0 &&
	pthread_create(&h,&attr,(void *(*)(void *))func,arg)==0 )
	return (void *)h;

  return NULL;
}
static void  __thread_yield(void)	{ (void)sched_yield(); }
static int   __thread_wait(void *h)
{ void *ret;
    if (pthread_join((pthread_t)((size_t)h),&ret)==0)
	return (int)ret;
  return -1;
}
static void *__semaphore_create(int max)
{ sem_t *h = calloc(1,sizeof(sem_t));
    if (h==NULL)		return h;
    if (sem_init(h,0,0)<0)	{ free(h); return NULL; }
  return h;
}
static int   __semaphore_wait(void *h)	{ return sem_wait(h)==0; }
static int   __semaphore_post(void *h)	{ return sem_post(h)==0; }
static void  __semaphore_close(void *h)	{ sem_destroy(h), free(h); }

#endif
