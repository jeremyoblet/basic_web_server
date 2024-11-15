#pragma once

#include <string>
#include "HttpRequestHandler.h"

class HttpGetHandler : public HttpRequestHandler
{
public:
    HttpGetHandler();
    virtual ~HttpGetHandler();

    void HandleRequest(
        const std::shared_ptr<ClientSocket>& client_socket,
        const std::shared_ptr<HttpData>& http_data) override;

private:
    void SendFileResponse(std::shared_ptr<ClientSocket> client_socket, const std::string& path);
};