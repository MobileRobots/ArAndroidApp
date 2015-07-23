/*
 Copyright 2011 Adept Technology, Inc.
*/

#include "pthread_android_additions.h"

/*
 Additions to pthread.c needed by Aria. These functions are supposed to be a 
 part of libc.so, but it appears they were left out. As a first pass I have
 commented out all code to get them to compile, and it actually appears that
 the arnetworking library still works fine. At some some point this code should
 be added back in.
*/

int pthread_condattr_init(pthread_condattr_t *attr)
{
  return 0;
}

int pthread_condattr_destroy(pthread_condattr_t *attr)
{
  return 0;
}

int pthread_setcancelstate(int state, int * oldstate)
{
  /*
  pthread_descr self = thread_self();
  if (state < PTHREAD_CANCEL_ENABLE || state > PTHREAD_CANCEL_DISABLE)
    return EINVAL;
  if (oldstate != NULL) *oldstate = THREAD_GETMEM(self, p_cancelstate);
  THREAD_SETMEM(self, p_cancelstate, state);
  if (THREAD_GETMEM(self, p_canceled) &&
      THREAD_GETMEM(self, p_cancelstate) == PTHREAD_CANCEL_ENABLE &&
      THREAD_GETMEM(self, p_canceltype) == PTHREAD_CANCEL_ASYNCHRONOUS)
    __pthread_do_exit(PTHREAD_CANCELED, CURRENT_STACK_FRAME);
  */
  return 0;
}

int pthread_setcanceltype(int type, int * oldtype)
{
	/*
  pthread_descr self = thread_self();
  if (type < PTHREAD_CANCEL_DEFERRED || type > PTHREAD_CANCEL_ASYNCHRONOUS)
    return EINVAL;
  if (oldtype != NULL) *oldtype = THREAD_GETMEM(self, p_canceltype);
  THREAD_SETMEM(self, p_canceltype, type);
  if (THREAD_GETMEM(self, p_canceled) &&
      THREAD_GETMEM(self, p_cancelstate) == PTHREAD_CANCEL_ENABLE &&
      THREAD_GETMEM(self, p_canceltype) == PTHREAD_CANCEL_ASYNCHRONOUS)
    __pthread_do_exit(PTHREAD_CANCELED, CURRENT_STACK_FRAME);
  */
  return 0;
}

int pthread_cancel(pthread_t thread)
{
/*
  pthread_handle handle = thread_handle(thread);
  int pid;
  int dorestart = 0;
  pthread_descr th;
  pthread_extricate_if *pextricate;
  int already_canceled;

  __pthread_lock(&handle->h_lock, NULL);
  if (invalid_handle(handle, thread)) {
    __pthread_unlock(&handle->h_lock);
    return ESRCH;
  }

  th = handle->h_descr;

  already_canceled = th->p_canceled;
  th->p_canceled = 1;

  if (th->p_cancelstate == PTHREAD_CANCEL_DISABLE || already_canceled) {
    __pthread_unlock(&handle->h_lock);
    return 0;
  }

  pextricate = th->p_extricate;
  pid = th->p_pid;
*/
  /* If the thread has registered an extrication interface, then
     invoke the interface. If it returns 1, then we succeeded in
     dequeuing the thread from whatever waiting object it was enqueued
     with. In that case, it is our responsibility to wake it up.
     And also to set the p_woken_by_cancel flag so the woken thread
     can tell that it was woken by cancellation. */
/*
  if (pextricate != NULL) {
    dorestart = pextricate->pu_extricate_func(pextricate->pu_object, th);
    th->p_woken_by_cancel = dorestart;
  }

  __pthread_unlock(&handle->h_lock);
*/
  /* If the thread has suspended or is about to, then we unblock it by
     issuing a restart, instead of a cancel signal. Otherwise we send
     the cancel signal to unblock the thread from a cancellation point,
     or to initiate asynchronous cancellation. The restart is needed so
     we have proper accounting of restarts; suspend decrements the thread's
     resume count, and restart() increments it.  This also means that suspend's
     handling of the cancel signal is obsolete. */
/*
  if (dorestart)
    restart(th);
  else
    kill(pid, __pthread_sig_cancel);
*/
  return 0;
}
