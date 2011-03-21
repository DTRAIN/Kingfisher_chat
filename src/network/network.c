#include "network.h"
#include "errors.h"
#include <stdio.h>
int clients[FD_SETSIZE];
char connections[FD_SETSIZE][IP_SIZE];
int totalclients = 0;
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: create_sock
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int create_sock(void)
  --
  -- RETURNS: the new sock.
  --
  -- NOTES:
  -- creates a new socket.
  ----------------------------------------------------------------------------------------------------------------------*/
int create_sock(void) {

    int sock;
    int opt = 0;
    int ret = -1;

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	    serv_err(SOCK_ERR, "socket");
    }
    
    //reuse addr incase of program exit.
    opt = 1;
    if((ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) == -1) {
	    serv_err(SOCK_ERR, "setsockopt");
    }

    //set socket buffer sizes.
    opt = BUFSIZE;
    if((ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt))) == -1) {
	    serv_err(SOCK_ERR, "setsockopt");
    }
    if((ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt)))) {
	    serv_err(SOCK_ERR, "setsockopt");
    }

    return sock;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: connect_client_sock
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int connect_client_sock(int sock, char* host_addr)
  --                                - sock        -- the socket to connect.
  --                                - servaddr    -- the address of the server to connect to.
  -- RETURNS: the socket after being connect to the server on success, 0 on failure to resolve hostname.
  --
  --
  -- NOTES:
  -- connects a socket to the server address specified
  ----------------------------------------------------------------------------------------------------------------------*/
int connect_client_sock(int sock, char* host_addr) {
    struct hostent *host;
    struct sockaddr_in server;

    //setup sock_addr struct.
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    //create hostent.
    if((host = gethostbyname(host_addr)) == NULL) {
        printf("failed to resolve hostname\n");
	    return 0;
    }

    memcpy(&(server.sin_addr), host->h_addr, host->h_length);
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        printf("connection refused by server\n");
	    return 0;
    }
    printf("connected to server\n");
    return sock;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: bind_server_sock
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int bind_server_sock(int sock)
  --                                - sock        -- the socket to connect.
  --
  --
  -- RETURNS: the socket after being bound.
  --
  --
  -- NOTES:
  -- binds a socket to a predefined port.
  ----------------------------------------------------------------------------------------------------------------------*/
int bind_server_sock(int sock) {
    struct sockaddr_in server;
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
	    serv_err(SOCK_ERR, "bind");
    }
    printf("bound to port %d\n", PORT);
    return sock;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: accept_connection
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int accept_connection(int newsock, int listensock)
  --                                - newsock        -- the accepted socket.
  --                                - listensock     -- the listening socket.
  --
  --
  -- RETURNS: the socket after being accepted.
  --
  --
  -- NOTES:
  -- accepts a connection request.
  ----------------------------------------------------------------------------------------------------------------------*/
int accept_connection(int newsock, int listensock) {

    struct sockaddr_in client;
    unsigned int len = sizeof(client);
    newsock = accept(listensock, (struct sockaddr*)&client, &len);
    printf("accepted connection from client at address: %s\n", inet_ntoa(client.sin_addr));
    return newsock;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: listen_server_sock
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int listen_server_sock(int sock)
  --                  - sock    -- the socket to be put in the listen state.
  --
  --
  -- RETURNS: the socket after being put into the listen state.
  --
  --
  -- NOTES:
  -- sets a socket in the listen state.
  ----------------------------------------------------------------------------------------------------------------------*/
int listen_server_sock(int sock) {
    if(listen(sock, MAXREQ) == -1) {
        serv_err(SOCK_ERR, "listen");
    }
    return sock;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: closeConnection
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void closeConnection(int sock)
  --                  - sock    -- the socket to be closed.
  --
  --
  -- RETURNS: void
  --
  --
  -- NOTES:
  -- sends a disconnect message to the server from a client, and closes the socket.
  ----------------------------------------------------------------------------------------------------------------------*/
void closeConnection(int sock) {
    char buf[PACKETSIZE];
    int i;
    memset(buf, 0, PACKETSIZE);
    strcpy(buf, "close");
    i = write(sock, buf, PACKETSIZE);
    close(sock);
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: send_packet
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int send_packet(int sock, char* packet)
  --                  - sock   -- the socket to send the packet across.
  --                  - packet -- the packet to be sent
  --
  --
  -- RETURNS: the return value of write.
  --
  --
  -- NOTES:
  -- sends a packet over a specified socket.
  ----------------------------------------------------------------------------------------------------------------------*/
int send_packet(int sock, char* packet) {
    return send(sock, packet, PACKETSIZE, 0);
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: recv_packet
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int recv_packet(int sock, char* buf)
  --                  - sock   -- the socket to read the packet from.
  --                  - buf    -- the buffer to be read into.
  --
  --
  -- RETURNS: the return value of read.
  --
  --
  -- NOTES:
  -- reads a packet from a specified socket.
  ----------------------------------------------------------------------------------------------------------------------*/
int recv_packet(int sock, char* buf) {
    int nRead = 0, toRead = PACKETSIZE;
    while((nRead = read(sock, buf, toRead)) > 0) {
        buf += nRead;
        toRead -= nRead;
    }
    return nRead;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: init_select
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void init_select(fd_set* set, int initsock)
  --                  - set         -- the file descriptor set to be initialized.
  --                  - initsock    -- the first socket to be initialized with the set.
  --
  --
  -- RETURNS: void
  --
  --
  -- NOTES:
  -- initializes and FD_SET to be used for select.
  ----------------------------------------------------------------------------------------------------------------------*/
void init_select(fd_set* set, int initsock) {
    int i;

    for (i = 0; i < FD_SETSIZE; i++) {
        clients[i] = -1;
    }

    FD_ZERO(set);
    FD_SET(initsock, set);
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: add_select_sock
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: int add_select_sock(fd_set* set, int addsock)
  --                  - set         -- the file descriptor set to have a socket added to.
  --                  - addsock     -- the socket to be added to the set.
  --
  --
  -- RETURNS: the number of clients stored in the set.
  --
  --
  -- NOTES:
  -- adds a socket to an FD_SET for select.
  ----------------------------------------------------------------------------------------------------------------------*/
int add_select_sock(fd_set* set, int addsock) {
    int i;
    size_t socklen;
    struct sockaddr_in sockname;

    for (i = 0; i < FD_SETSIZE; i++) {
        if (clients[i] < 0) {
            getsockname(addsock, (struct sockaddr*)&sockname, &socklen);
            strcpy(connections[i], inet_ntoa(sockname.sin_addr));
            clients[i] = addsock;
            break;
        }
    }

    if(i == FD_SETSIZE) {
        serv_err(SELECT_ERR, "select");
    }
    FD_SET (addsock, set);

    return i;
}
/*------------------------------------------------------------------------------------------------------------------
  -- FUNCTION: remove_select_sock
  --
  -- DATE: March 20, 2011
  --
  -- REVISIONS: (Date and Description)
  --
  -- DESIGNER: Duncan Donaldson.
  --
  -- PROGRAMMER: Duncan Donaldson.
  --
  -- INTERFACE: void remove_select_sock(fd_set* set, int rmsock, int i)
  --                  - set         -- the file descriptor set to have a socket removed from.
  --                  - rmsock      -- the socket to be removed from the set.
  --
  --
  -- RETURNS: void
  --
  --
  -- NOTES:
  -- removes a socket from an FD_SET for select.
  ----------------------------------------------------------------------------------------------------------------------*/
void remove_select_sock(fd_set* set, int rmsock, int i) {

    printf("session terminated by client\n");
    close(rmsock);
    FD_CLR(rmsock, set);
    memset(connections[i], 0, IP_SIZE);
    clients[i] = -1;
    totalclients--;

    print_clients();
}

void print_clients() {
    int i;
    printf("all currently connected clients:\n");
    for(i = 0; i < totalclients; ++i) {
        printf("%s\n", connections[i]);
    }
}
