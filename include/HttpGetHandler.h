#pragma once

#include "HttpRequestHandler.h"

#include <memory>
#include <string>

class HttpGetHandler : public HttpRequestHandler
{
public:
    HttpGetHandler() = default;
    ~HttpGetHandler() override = default;

    void HandleRequest(
        const std::shared_ptr<ClientSocket>& client_socket,
        const std::shared_ptr<HttpData>& http_data) override;

private:
    void SendFileResponse(std::shared_ptr<ClientSocket> client_socket, const std::string& path);
};