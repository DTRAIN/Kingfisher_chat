#include "errors.h"
void serv_err(int retval, char* msg) {
    perror(msg);
    exit(retval);
}
