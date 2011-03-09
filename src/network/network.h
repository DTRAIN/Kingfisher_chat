#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 10127
#define PACKETSIZE 4096
#define BUFSIZE ((PACKETSIZE)*(10))
#define MAXREQ 5
int create_sock(void);
int connect_client_sock(int sock, char* host_addr);
int bind_server_sock(int sock);
int listen_server_sock(int sock);
int accept_connection(int sock);
int send_packet(int sock, char* packet);
void recv_packet(int sock, char* buf);
void init_select(fd_set* set, int* clients, int initsock);
void add_select_sock(fd_set* set, int* clients, int addsock);
#endif
