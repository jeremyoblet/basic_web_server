#pragma once

#include "ClientSocket.h"
#include "HttpData.h"

#include <memory>


class HttpRequestHandler {
public:
    virtual ~HttpRequestHandler() = default;

    void HandleRequest(int client_socket);

    virtual void HandleRequest(
        const std::shared_ptr<ClientSocket>& client_socket,
        const std::shared_ptr<HttpData>& http_data
        ) = 0;

        
    void handle_get(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data);
    void handle_post(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data);
    void handle_put(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data);
    void handle_delete(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data);
    void handle_head(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data);

    void handle_cgi(std::shared_ptr<ClientSocket> client_socket, const std::shared_ptr<HttpData>& http_data);

    void Send404Response(std::shared_ptr<ClientSocket> client_socket);
    void Send408Response(std::shared_ptr<ClientSocket> client_socket);
    void Send409Response(std::shared_ptr<ClientSocket> client_socket);
    void Send400Response(std::shared_ptr<ClientSocket> client_socket);
    void Send500Response(std::shared_ptr<ClientSocket> client_socket);
};
