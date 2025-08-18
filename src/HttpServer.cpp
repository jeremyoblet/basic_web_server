#include "HttpServer.h"
#include "HttpRequestHandler.h"
#include <stdexcept>
#include <iostream>

#ifdef _WIN32
  #define CLOSE_SOCKET(s) closesocket(s)
  static void INIT_SOCKET(){ WSADATA w; if (WSAStartup(MAKEWORD(2,2), &w)!=0) throw std::runtime_error("WSAStartup failed"); }
  static void CLEANUP_SOCKET(){ WSACleanup(); }
#else
  #define CLOSE_SOCKET(s) close(s)
  static void INIT_SOCKET(){}
  static void CLEANUP_SOCKET(){}
#endif

HttpServer::HttpServer(int p, size_t n) : port(p), thread_pool(n) {}
HttpServer::~HttpServer(){ stop(); }

void HttpServer::start() {
    if (is_running.exchange(true)) return;
    INIT_SOCKET();

    server_socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET_VALUE) { is_running=false; CLEANUP_SOCKET(); throw std::runtime_error("socket() failed"); }

    int yes = 1;
    (void)setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    sockaddr_in addr{};                       // zero-init
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (::bind(server_socket, (sockaddr*)&addr, sizeof(addr)) < 0) { stop(); throw std::runtime_error("bind() failed"); }
    if (::listen(server_socket, SOMAXCONN) < 0) { stop(); throw std::runtime_error("listen() failed"); }

    while (is_running.load()) {
    #ifdef _WIN32
        SOCKET client = ::accept(server_socket, nullptr, nullptr);
        if (client == INVALID_SOCKET) { if (!is_running) break; continue; }
    #else
        int client = ::accept(server_socket, nullptr, nullptr);
        if (client < 0) { if (!is_running) break; continue; }
    #endif
        thread_pool.submit([client]{
            HttpRequestHandler router;           // routeur
            router.HandleRequest((int)client);   // ferme le client dans le handler
            // Si vos handlers ne ferment pas le socket client, décommentez la ligne suivante :
            // CLOSE_SOCKET(client);
        });
    }
}

void HttpServer::stop() {
    if (!is_running.exchange(false)) return;
    if (server_socket != INVALID_SOCKET_VALUE) {
        CLOSE_SOCKET(server_socket);             // débloque accept()
        server_socket = INVALID_SOCKET_VALUE;
    }
    CLEANUP_SOCKET();
}
