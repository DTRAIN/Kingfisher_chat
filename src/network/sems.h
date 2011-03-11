#ifndef SEMS_H
#define SEMS_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#define SEM_KEY (key_t)1234567
int initsem (void);
void P(int sid);
void V(int sid);
int get_sem_val(int sid);
#endif
