#include "network.h"

void* echo_thread(void*);

int main(void) {
    int listensock, nextsock, acceptsock,
	readsock, echosock;
    int totalclients, numselected;
    int clients[FD_SETSIZE];
    fd_set readyset, allset;
    struct sockaddr_in* client;
    
    //create bind and listen on lsock.
    listensock = create_sock();
    listensock = bind_server_sock(listensock);
    listensock = listen_server_sock(listensock);

    //setup for select
    nextsock = listensock;
    totalclients = -1;
    init_select(&allset, clients, listensock);

    while(TRUE) {

	readyset = allset;
	numselected = select(nextsock+1, &readyset, NULL, NULL, NULL);

	if(FD_ISSET(listensock, &rset)) {

	    int i;

	    if ((acceptsock = accept_connection(acceptsock, listensock)) == -1) {
		serv_error(SOCK_ERR, "accept");
	    }

	    i = add_select_sock(&allset, clients, acceptsock);

	    if (acceptsock > nextsock) {
		nextsock = acceptsock;
	    }
	    if (i > totalclients) {
		totalclients = i;
	    }
	    if (--numselected <= 0) {
		continue;
	    }

	}

	for (i = 0; i <= totalclients; i++) {

	    if (clients[i] >= 0) {
		readsock = clients[i];
	    } else {
		continue;
	    }

	    if (FD_ISSET(readsock, &readyset)) {
		char buf[PACKETSIZE];

		recv_packet(readsock, buf);
				
		//write to echo thread
									            				
		if (--numselected <= 0) {
		    break;
		}
	    }
	}

    }
}

void* echo_thread(void*) {

    int ipcsocket;

    ipcsocket = create_ipc_socket;
    ipcsocket = bind_ipc_socket;
    
    while(1) {
	char buf[PACKETSIZE];
	if(read(ipcsocket, buf, PACKETSIZE) > 0) {
	    for(int i = 0; clients[i] != -1; ++i) {
		send_packet(clients[i], buf);
	    }
	}
    }

    return 0;
}
