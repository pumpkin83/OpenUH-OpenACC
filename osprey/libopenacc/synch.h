#ifndef __SYNCH_H__
#define __SYNCH_H__

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

typedef void* semaphore_t;

extern semaphore_t semaphore_create();

extern void semaphore_initialize(semaphore_t p_semaphore, int count);

extern void semaphore_P(semaphore_t p_semaphore);

extern void semaphore_V(semaphore_t p_semaphore);

extern void semaphore_destroy(semaphore_t p_semaphore);

#endif
