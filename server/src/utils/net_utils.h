#ifndef NET_UTILS
#define NET_UTILS 1

#include "../include/common.h"
#include "../clients/connection.h"

void sockaddr_to_string(char *str_addr, struct sockaddr *sockaddr);
bool test_hangup(int socket_fd);

void readall_nio(connection_t *connection);
void sendall_nio(connection_t *connection);

void shutdown_w(int sock_fd);

#endif
