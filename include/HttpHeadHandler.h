#pragma once

#include "HttpRequestHandler.h"


class HttpHeadHandler: public HttpRequestHandler
{
public:
    HttpHeadHandler() = default;
    ~HttpHeadHandler() override = default;

    void HandleRequest(const std::shared_ptr<ClientSocket>& client_socket, const std::shared_ptr<HttpData>& http_data) override;

private:

};