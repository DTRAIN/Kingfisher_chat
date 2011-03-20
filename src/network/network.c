#include "network.h"
#include "errors.h"
#include <stdio.h>
int clients[FD_SETSIZE];
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

int connect_client_sock(int sock, char* host_addr) {
    struct hostent *host;
    struct sockaddr_in server;

    //setup sock_addr struct.
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    //create hostent.
    if((host = gethostbyname(host_addr)) == NULL) {
	return 0;
    }

    memcpy(&(server.sin_addr), host->h_addr, host->h_length);
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
	serv_err(SOCK_ERR, "connect");
    }
    
    return sock;
}

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

int accept_connection(int newsock, int listensock) {

    struct sockaddr_in client;
    unsigned int len = sizeof(client);
    newsock = accept(listensock, (struct sockaddr*)&client, &len);
    printf("accepted connection from client at address: %s\n", inet_ntoa(client.sin_addr));
    return newsock;
}

int listen_server_sock(int sock) {
    if(listen(sock, MAXREQ) == -1) {
        serv_err(SOCK_ERR, "listen");
    }
    return sock;
}

int send_packet(int sock, char* packet) {
    return send(sock, packet, PACKETSIZE, 0);
}

int recv_packet(int sock, char* buf) {
    int nRead = 0, toRead = PACKETSIZE;
    while((nRead = read(sock, buf, toRead)) > 0) {
        buf += nRead;
        toRead -= nRead;
    }
    if(nRead == -1) {
        return 0;
    }
    return 1;
}

void init_select(fd_set* set, int initsock) {
    int i;

    for (i = 0; i < FD_SETSIZE; i++) {
        clients[i] = -1;
    }

    FD_ZERO(set);
    FD_SET(initsock, set);
}

int add_select_sock(fd_set* set, int addsock) {
    int i;

    for (i = 0; i < FD_SETSIZE; i++) {
        if (clients[i] < 0) {
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
