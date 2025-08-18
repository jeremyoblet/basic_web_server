#pragma once

#include "HttpRequestHandler.h"


class HttpPostHandler: public HttpRequestHandler
{
public:
    HttpPostHandler() = default;
    ~HttpPostHandler() override = default;

    void HandleRequest(
        const std::shared_ptr<ClientSocket>& client_socket,
        const std::shared_ptr<HttpData>& http_data
        ) override;

private:

};
