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
#include <fcntl.h>
#include "errors.h"
#define IP_SIZE 16
#define PORT 10127
#define PACKETSIZE 4096
#define BUFSIZE ((PACKETSIZE)*(10))
#define MAXREQ 5
int create_sock(void);
int connect_client_sock(int sock, char* host_addr);
int bind_server_sock(int sock);
int listen_server_sock(int sock);
int accept_connection(int newsock, int listensock);
int send_packet(int sock, char* packet);
int recv_packet(int sock, char* buf);
void init_select(fd_set* set, int initsock);
int add_select_sock(fd_set* set, int addsock);
void closeConnection(int sock);
void remove_select_sock(fd_set* set, int rmsock, int i);
void add_connection(char* claddr);
void rm_connection(char* claddr);
void print_connections(void);
void sig_handler(int);
int open_log_file(void);
void log_data(char* data, int fd);
void close_log_file(int fd);
void echo(char* data);
#endif
