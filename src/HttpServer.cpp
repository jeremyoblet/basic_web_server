#include "HttpServer.h"

#ifdef _WIN32

#define CLOSE_SOCKET(s) closesocket(s)
#define INIT_SOCKET() WSADATA wsaData; WSAStartup(MAKEWORD(2,2), &wsaData)
#else
#define CLOSE_SOCKET(s) close(s)
#define INIT_SOCKET() // Nothing needed for Unix
#endif

HttpServer::HttpServer(int port, size_t num_threads) : port(port), thread_pool(num_threads), is_running(false) {}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::start() {
    is_running = true;

    INIT_SOCKET();

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        throw std::runtime_error("Socket failed");
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(server_socket, SOMAXCONN) < 0) {
        throw std::runtime_error("Listen failed");
    }

    while (is_running)
    {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket >= 0) {
            thread_pool.submit([this, client_socket] {
                HttpRequestHandler().HandleRequest(client_socket);
            });
            std::cout << "Done with socket " << client_socket << std::endl;
        }
    }
}

void HttpServer::stop() {
    is_running = false;
    if (server_socket != -1) {
        CLOSE_SOCKET(server_socket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}