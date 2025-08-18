#pragma once
#include "HttpRequestHandler.h"

class HttpPostHandler : public HttpRequestHandler {
public:
    HttpPostHandler() = default;
    ~HttpPostHandler() override = default;

    void HandleRequest( // DOIT être public + override
        const std::shared_ptr<ClientSocket>& client,
        const std::shared_ptr<HttpData>& req) override;
};
