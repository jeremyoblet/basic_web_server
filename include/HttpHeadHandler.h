#pragma once

#include "HttpRequestHandler.h"


class HttpHeadHandler: public HttpRequestHandler
{
public:
    HttpHeadHandler() = default;
    ~HttpHeadHandler() override = default;

    void HandleRequest(const std::shared_ptr<ClientSocket>&, const std::shared_ptr<HttpData>&) override;

// private:
//     void SendFileResponse(std::shared_ptr<ClientSocket>, const std::string& path);
};