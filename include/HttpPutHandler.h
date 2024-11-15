#pragma once

#include <HttpRequestHandler.h>

class HttpPutHandler: public HttpRequestHandler
{
public:
    HttpPutHandler();
    virtual ~HttpPutHandler();

    void HandleRequest(
        const std::shared_ptr<ClientSocket>& client_socket,
        const std::shared_ptr<HttpData>& http_data
        ) override;
private:

};
