/*
 Copyright 2011 Adept Technology, Inc.
*/

#ifndef PTHREAD_ANDROID_ADD_H
#define PTHREAD_ANDROID_ADD_H

#include "pthread.h"

/*
 Additions to pthread.c needed by Aria. These functions are supposed to be a 
 part of libc.so, but it appears they were left out. As a first pass I have
 commented out all code to get them to compile, and it actually appears that
 the arnetworking library still works fine. At some some point this code should
 be added back in.
*/

#define PTHREAD_CANCEL_ENABLE					(0)
#define PTHREAD_CANCEL_ASYNCHRONOUS		(0)

int pthread_condattr_init(pthread_condattr_t *attr);

int pthread_condattr_destroy(pthread_condattr_t *attr);

int pthread_setcancelstate(int state, int * oldstate);

int pthread_setcanceltype(int type, int * oldtype);

int pthread_cancel(pthread_t thread);

#endif
