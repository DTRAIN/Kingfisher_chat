#include "network.h"

int main(void) {
    int lsock, nsock, asock;
    int max_clients, numselected;
    int clients[FD_SETSIZE];
    fd_set rset, allset;
    struct sockaddr_in* client;
    
    //create bind and listen on lsock.
    lsock = create_sock();
    lsock = bind_server_sock(lsock);
    lsock = listen_server_sock(lsock);
    
    if(lsock == 0) {
	serv_error(SOCK_ERR, "create, or bind, or listen");	
    }

    //setup for select
    nsock = lsock;
    max_clients = -1;
    init_select(&allset, clients, lsock);

    while(TRUE) {

	rset = allset;
	numselected = select(nsock+1, &rset, NULL, NULL, NULL);

	if(FD_ISSET(lsock, &rset)) {

	    int i;

	    if ((asock = accept_connection(asock, lsock)) == -1) {
		serv_error(SOCK_ERR, "accept");
	    }

	    i = add_select_sock(&allset, clients, asock);

	    if (asock > nsock) {
		nsock = asock;
	    }
	    if (i > max_clients) {
		max_clients = i;
	    }
	    if (--numselected <= 0) {
		continue;
	    }

	}
	
    }
}
