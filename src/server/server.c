#include "../network/network.h"
#include "../network/errors.h"
#include "../network/sems.h"
void echo(char* data);
extern int totalclients;
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

	    if ((readsock = clients[j]) < 0) {
	        continue;
	    }

	    if (FD_ISSET(readsock, &readyset)) {
		    char buf[PACKETSIZE];
		    ssize_t n;
		    n = recv_packet(readsock, buf);
		    if(strcmp(buf, "close") == 0) {
                remove_select_sock(&allset, readsock, j);
            }
		    printf("received message\n");
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
    struct sockaddr_in sock;
    size_t socklen;
    for(i = 0; i < FD_SETSIZE; ++i) {
	if(clients[i] > 0) {
	  send_packet(clients[i], data);
	  getsockname(clients[i], (struct sockaddr*)&sock, &socklen);
	  printf("echoed to client %s\n", inet_ntoa(sock.sin_addr));
	}
    }
    
}
