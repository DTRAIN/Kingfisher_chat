#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "sems.h"
#include "errors.h"
#define PORT 10127
#define PACKETSIZE 4096
#define BUFSIZE ((PACKETSIZE)*(10))
#define MAXREQ 5
int create_sock(void);
int create_ipc_sock(void);
int bind_ipc_sock(int sock);
ssize_t send_ipc_packet(int sock, char* buf);
ssize_t read_ipc_packet(int sock, char* buf);
int connect_client_sock(int sock, char* host_addr);
int bind_server_sock(int sock);
int listen_server_sock(int sock);
int accept_connection(int newsock, int listensock);
int send_packet(int sock, char* packet);
int recv_packet(int sock, char* buf);
void init_select(fd_set* set, int* clients, int initsock);
int add_select_sock(fd_set* set, int* clients, int addsock);
#endif
