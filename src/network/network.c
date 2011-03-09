#include "network.h"

int create_sock(void) {

    int sock;
    int opt = 0;
    int ret = -1;

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	return 0;
    }
    
    //reuse addr incase of program exit.
    opt = 1;
    ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //set socket buffer sizes.
    opt = BUFSIZE;
    ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt));
    ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt));

    if(ret == -1) {
	close(sock);
	return 0;
    }

    return sock;
}

int connect_client_sock(int sock, char* host_addr) {
    struct hostent *host;
    struct sockaddr_int server;

    //setup sock_addr struct.
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    //create hostent.
    if(host = gethostbyname(host_addr) == NULL) {
	return 0;
    }

    memcpy(&(server.sin_addr), host->h_addr, host->h_len);
    if(connect(sock, (struct sockaddr*)server, sizeof(server)) == -1) {
	return 0;
    }
    
    return sock;
}

int bind_server_sock(int sock) {
    struct sockaddr_in* server;
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
	return 0;
    }
    
    return sock;
}


int listen_server_sock(int sock) {
    if(listen(sock, MAXREQ) == -1) {
	return 0;
    }
    return sock;
}

int send_packet(int sock, char* packet) {
    return write(sock, packet, PACKETSIZE);
}

int recv_packet(int sock, char* buf) {
    int nRead = 0, toRead = PACKETSIZE;
    while((nRead = read(sock, buf, toRead)) > 0) {
	buf += nRead;
	toRead -= nRead;
    }
    return nRead;
}
