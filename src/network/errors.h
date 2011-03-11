#ifndef ERRORS_H
#define ERRORS_H
#define SOCK_ERR 1
#define READ_ERR 2
#define WRITE_ERR 3
#define SELECT_ERR 4
#define SEM_ERR 5
#define THREAD_ERR 6
#include <stdlib.h>
#include <stdio.h>
void serv_err(int retval, char* msg);
#endif
