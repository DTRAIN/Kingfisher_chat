#include "../network/network.h"
#include "../network/errors.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define server main

//a couple globals :(
extern int totalclients;
extern int clients[FD_SETSIZE];
/*------------------------------------------------------------------------------------------------------------------
--  SOURCE FILE: main.c - A simple chat echo server. This server is part of a simple chat echo client / server application.
--                        
--  PROGRAM: server
--
--  FUNCTIONS:
--  ***************SERVER*********************
--  int server(void)
--  void echo(char*)
--  ******************************************
--  ****************NETWORK*******************
--  int create_sock(void);
--  int bind_server_sock(int)
--  int listen_server_sock(int)
--  int send_packet(int, char*)
--  int recv_packet(int, char*)
--  int accept_connection(int, int)
--  void init_select(fd_set*, int)
--  int add_select_sock(fd_set*, int)
--  void remove_select_sock(fd_set*, int, int)
--  ******************************************
--  ****************ERRORS********************
--  void serv_err(int, char*)
--  ******************************************       
--
--
--  DATE: March 20, 2011
--
--  REVISIONS: (Date and Description)
--
--  DESIGNER: Duncan Donaldson
--
--  PROGRAMMER: Duncan Donaldson
--
--  NOTES: This program is a simple chat echo server, it handles connection requests and messages from clients via select.
--         when a message is received, the server echoes it back out to all currently connected clients.
----------------------------------------------------------------------------------------------------------------------*/
int server(void) {

    int listensock, nextsock, acceptsock = 0, readsock;
    int numselected;   
    fd_set readyset, allset;
    
    //handle signals
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

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
        struct sockaddr_in sock;
        size_t socklen;
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

            getsockname(nextsock, (struct sockaddr*)&sock, &socklen);
            add_connection(inet_ntoa(sock.sin_addr));
            print_connections();

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
                
                //blank buffer
                memset(buf, 0, PACKETSIZE);
                //read packet
		        n = recv_packet(readsock, buf);
                

                //if close message, remove socket.
		        if(strcmp(buf, "close") == 0) {
                    
                    //get connection info, add connection to list
                    getsockname(readsock, (struct sockaddr*)&sock, &socklen);
                    rm_connection(inet_ntoa(sock.sin_addr));
                    print_connections();

		            remove_select_sock(&allset, readsock, j);

		        } else {
                    //otherwise echo
		            printf("received message\n");
		            echo(buf);

                }
	            if (--numselected <= 0) {
	                break;
	            }
	        }
	    }
    }
}

