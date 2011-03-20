#include "../network/network.h"
#include "../network/errors.h"
#include "../network/sems.h"
void echo(char* data);
int totalclients = 0;
extern int clients[FD_SETSIZE];
int main(void) {

    int listensock, nextsock, acceptsock = 0,
	readsock;
    int numselected;
    fd_set readyset, allset;
    
    //create bind and listen on lsock.
    listensock = create_sock();
    listensock = bind_server_sock(listensock);
    listensock = listen_server_sock(listensock);
    
    //setup for select
    nextsock = listensock;
    totalclients = -1;
    init_select(&allset, listensock);

    while(1) {

	int j;
	readyset = allset;
	numselected = select(nextsock+1, &readyset, NULL, NULL, NULL);

	if(FD_ISSET(listensock, &readyset)) {

	    int i;

	    if ((acceptsock = accept_connection(acceptsock, listensock)) == -1) {
		serv_err(SOCK_ERR, "accept");
	    }

            i = add_select_sock(&allset, acceptsock);

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

	for (j = 0; j <= totalclients; j++) {

	    if (clients[j] >= 0) {
		readsock = clients[j];
	    } else {
		continue;
	    }

	    if (FD_ISSET(readsock, &readyset)) {
		char buf[PACKETSIZE];

		recv_packet(readsock, buf);

		echo(buf);
		if (--numselected <= 0) {
		    break;
		}
	    }
	}
    }
}

void echo(char* data) {

    int i;

    for(i = 0; i < totalclients; ++i) {
        send_packet(clients[i], data);
    }
    
}
