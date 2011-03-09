#ifndef ERRORS_H
#define ERRORS_H
#define SOCK_ERR 1
#define READ_ERR 2
#define WRITE_ERR 3
void serv_err(int retval, char* msg);
#endif
