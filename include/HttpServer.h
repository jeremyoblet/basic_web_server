#pragma once
#include "ThreadPool.h"
#include <atomic>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  using socket_t = SOCKET;
  static constexpr socket_t INVALID_SOCKET_VALUE = INVALID_SOCKET;
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  using socket_t = int;
  static constexpr socket_t INVALID_SOCKET_VALUE = -1;
#endif

class HttpServer {
public:
    HttpServer(int port, size_t num_threads);
    ~HttpServer();
    void start();
    void stop();

private:
    int port;
    socket_t server_socket = INVALID_SOCKET_VALUE;
    ThreadPool thread_pool;
    std::atomic<bool> is_running{false};
};
