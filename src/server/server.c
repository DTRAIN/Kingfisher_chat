#include "network.h"

int main(void) {
    int lsock, max_sockets;
    int clients[FD_SETSIZE];
    fd_set rset, allset;
    
    //create bind and listen on lsock.
    lsock = create_sock();
    lsock = bind_server_sock(lsock);
    lsock = listen_server_sock(lsock);
    
    if(lsock == 0) {
	return 1;
    }

}
