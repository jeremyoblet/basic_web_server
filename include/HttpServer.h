#pragma once

#include <iostream>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#endif

#include "ThreadPool.h"
#include "HttpRequestHandler.h"

class HttpServer {
public:
    HttpServer(int port, size_t num_threads);

    ~HttpServer();

    void start();

    void stop();

private:
    int port;
#ifdef _WIN32
    SOCKET server_socket;
#else
    int server_socket;
#endif
    ThreadPool thread_pool;
    bool is_running;
};
