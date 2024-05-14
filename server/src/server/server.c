#include "server.h"
#include "../utils/logger.h"
#include "../utils/net_utils.h"
#include "../utils/generic_utils.h"

static server_t *server = NULL;

server_t *get_server()
{
    return server;
}

static void init_server_handle(server_t **server_handle, int server_fd)
{
    server_t *new_server_handle = calloc_w(1, sizeof(server_t));
    new_server_handle->running = true;
    new_server_handle->server_fd = server_fd;

    *server_handle = new_server_handle;
}

static int init_server_socket(sockaddr_in_t sockaddr)
{
    int server_socket = socket(
        sockaddr.sin_family,
        SOCK_STREAM,
        IPPROTO_TCP
    );
    if (server_socket == ERR) {
        putlog("Error while getting server socket: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Reuse port */
    int yes = 1;
    int setopt_res = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (setopt_res == ERR) {
        putlog("Error while setting socket options: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Non blocking */
    int setnonblock_res = fcntl(server_socket, F_SETFL, O_NONBLOCK);
    if (setnonblock_res == ERR) {
        putlog("Error while setting socket to non blocking: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

void server_init(char *port, int backlog)
{
    uint16_t port_num = (uint16_t) atoi(port);

    sockaddr_in_t sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port_num);
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    mclear(sockaddr.sin_zero, sizeof(sockaddr.sin_zero));

    putlog("Process ID: %d", getpid());
    putlog("Starting at port: %hu", port_num);

    int server_socket = init_server_socket(sockaddr);

    int bind_res = bind(
        server_socket,
        (sockaddr_t *) &sockaddr,
        sizeof(sockaddr)
    );
    if (bind_res == ERR) {
        putlog("Error while binding server socket to port %s: %s", port, strerror(errno));
        exit(EXIT_FAILURE);
    }

    int listen_res = listen(server_socket, backlog);
    if (listen_res == ERR) {
        putlog("Error while calling listen: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    putlog("Server successfully initialized");
    putlog("Accepting connections...");

    init_server_handle(&server, server_socket);
}
