#pragma once

#include "HttpRequestHandler.h"


class HttpPutHandler: public HttpRequestHandler
{
public:
    HttpPutHandler() = default;
    ~HttpPutHandler() override = default;

    void HandleRequest(const std::shared_ptr<ClientSocket>&, const std::shared_ptr<HttpData>&) override;

private:
    void SendFileResponse(std::shared_ptr<ClientSocket>, const std::string& path);
};