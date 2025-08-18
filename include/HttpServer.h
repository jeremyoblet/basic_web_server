#pragma once

#ifdef _WIN32
    #include <WinSock2.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <errno.h>
#endif

#include "HttpRequestHandler.h"
#include "ThreadPool.h"

#include <atomic>
#include <iostream>


class HttpServer {

public:
    HttpServer(int port, size_t num_threads);
    ~HttpServer();
    void start();
    void stop();

private:
    int port;
#ifdef _WIN32
    SOCKET server_socket = INVALID_SOCKET;
#else
    int server_socket = -1;
#endif
    ThreadPool thread_pool;
    std::atomic<bool> is_running{false};
};