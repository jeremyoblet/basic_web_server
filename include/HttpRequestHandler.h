#pragma once

#include "ClientSocket.h"
#include "HttpData.h"

#include <memory>
#include <string>


class HttpRequestHandler {
public:
    virtual ~HttpRequestHandler() = default;

    void HandleRequest(int client_socket);

protected:
    virtual void HandleRequest(
        const std::shared_ptr<ClientSocket>& client_socket,
        const std::shared_ptr<HttpData>& http_data) {}

        
    void handle_get(std::shared_ptr<ClientSocket>, const std::shared_ptr<HttpData>&);
    void handle_post(std::shared_ptr<ClientSocket>, const std::shared_ptr<HttpData>&);
    void handle_put(std::shared_ptr<ClientSocket>, const std::shared_ptr<HttpData>&);
    void handle_delete(std::shared_ptr<ClientSocket>, const std::shared_ptr<HttpData>&);
    void handle_head(std::shared_ptr<ClientSocket>, const std::shared_ptr<HttpData>&);
    void handle_cgi(std::shared_ptr<ClientSocket>, const std::shared_ptr<HttpData>&);

    void Send404Response(std::shared_ptr<ClientSocket>);
    void Send405Response(std::shared_ptr<ClientSocket>);
    void Send408Response(std::shared_ptr<ClientSocket>);
    void Send409Response(std::shared_ptr<ClientSocket>);
    void Send400Response(std::shared_ptr<ClientSocket>);
    void Send500Response(std::shared_ptr<ClientSocket>);
};
