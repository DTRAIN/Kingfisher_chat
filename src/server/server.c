#include "../network/network.h"
#include "../network/errors.h"
#include "../network/sems.h"
#include <pthread.h>
void* echo_thread(void*);
int clients[FD_SETSIZE];

int main(void) {

    int listensock, nextsock, acceptsock = 0,
	readsock, echosock;
    int totalclients, numselected, sid;
    pthread_t tid;
    fd_set readyset, allset;
    
    //create bind and listen on lsock.
    listensock = create_sock();
    listensock = bind_server_sock(listensock);
    listensock = listen_server_sock(listensock);
    
    //setup for select
    nextsock = listensock;
    totalclients = -1;
    init_select(&allset, clients, listensock);

    //create semaphore
    sid = initsem();

    //create IPC socket
    echosock = create_ipc_sock();

    //create thread
    if(pthread_create(&tid, NULL, echo_thread, NULL) != 0) {
	serv_err(THREAD_ERR, "pthread_create");
    }

    while(1) {

	int j;
	readyset = allset;
	numselected = select(nextsock+1, &readyset, NULL, NULL, NULL);

	if(FD_ISSET(listensock, &readyset)) {

	    int i;

	    if ((acceptsock = accept_connection(acceptsock, listensock)) == -1) {
		serv_err(SOCK_ERR, "accept");
	    }
	    P(sid);
	    i = add_select_sock(&allset, clients, acceptsock);
	    V(sid);
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
		P(sid);
		recv_packet(readsock, buf);
		V(sid);	
		send_ipc_packet(echosock, buf);
		if (--numselected <= 0) {
		    break;
		}
	    }
	}

    }
}

void* echo_thread(void* arg) {

    int ipcsocket, sid;
    ipcsocket = create_ipc_sock();
    ipcsocket = bind_ipc_sock(ipcsocket);
    
    sid = initsem();

    while(1) {
	char buf[PACKETSIZE];
	if(read_ipc_packet(ipcsocket, buf) > 0) {
	    int i;
	    for(i = 0; clients[i] != -1; ++i) {
		P(sid);
		send_packet(clients[i], buf);
		V(sid);
	    }
	}
    }

    return arg;
}
