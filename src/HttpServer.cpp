#include "HttpServer.h"
#include "HttpRequestHandler.h"

#ifdef _WIN32
    #define CLOSE_SOCKET(s) closesocket(s)
    #define INIT_SOCKET() WSADATA wsaData; WSAStartup(MAKEWORD(2,2), &wsaData)
#else
    #define CLOSE_SOCKET(s) close(s)
    #define INIT_SOCKET() // Nothing needed for Unix
#endif


HttpServer::HttpServer(int port, size_t num_threads)
    : port(port)
    , thread_pool(num_threads)
    , is_running(false)
{}

HttpServer::~HttpServer() {
    stop();
}

// void HttpServer::start() {
//     is_running = true;

//     INIT_SOCKET();

//     server_socket = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_socket < 0) {
//         throw std::runtime_error("Socket failed");
//     }

//     sockaddr_in address;
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(port);

//     if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
//         throw std::runtime_error("Bind failed");
//     }

//     if (listen(server_socket, SOMAXCONN) < 0) {
//         throw std::runtime_error("Listen failed");
//     }

//     while (is_running)
//     {
//         int client_socket = accept(server_socket, nullptr, nullptr);
//         if (client_socket >= 0) {
//             thread_pool.submit([this, client_socket] {
//                 HttpRequestHandler().HandleRequest(client_socket);
//             });
//             std::cout << "Done with socket " << client_socket << std::endl;
//         }
//     }
// }

// void HttpServer::stop() {
//     is_running = false;
//     if (server_socket != -1) {
//         CLOSE_SOCKET(server_socket);
//     }
// #ifdef _WIN32
//     WSACleanup();
// #endif
// }

void HttpServer::start() {
    if (is_running.exchange(true)) return;

#ifdef _WIN32
    WSADATA w; if (WSAStartup(MAKEWORD(2,2), &w) != 0) { is_running=false; throw std::runtime_error("WSAStartup failed"); }
#endif

    server_socket = ::socket(AF_INET, SOCK_STREAM, 0);
#ifdef _WIN32
    if (server_socket == INVALID_SOCKET)
#else
    if (server_socket < 0)
#endif
    { stop(); throw std::runtime_error("socket() failed"); }

    int yes = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    sockaddr_in addr{};                  // zero-init
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
        thread_pool.submit([client] {
            HttpRequestHandler handler;
            handler.HandleRequest((int)client);
            // Fermez ici si ClientSocket/handler ne ferme pas:
            // #ifdef _WIN32
            // closesocket(client);
            // #else
            // close(client);
            // #endif
        });
    }
}

void HttpServer::stop() {
    if (!is_running.exchange(false)) return;
#ifdef _WIN32
    if (server_socket != INVALID_SOCKET) { closesocket(server_socket); server_socket = INVALID_SOCKET; }
    WSACleanup();
#else
    if (server_socket != -1) { close(server_socket); server_socket = -1; }
#endif
}
