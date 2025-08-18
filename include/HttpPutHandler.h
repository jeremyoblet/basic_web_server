#pragma once

#include "HttpRequestHandler.h"


class HttpPutHandler: public HttpRequestHandler
{
public:
    HttpPutHandler() = default;
    ~HttpPutHandler() override = default;

    void HandleRequest(
        const std::shared_ptr<ClientSocket>& client_socket,
        const std::shared_ptr<HttpData>& http_data
        ) override;
private:

};
