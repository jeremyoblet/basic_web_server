#pragma once

#include "HttpRequestHandler.h"

class HttpCGIHandler: public HttpRequestHandler
{
public:
    HttpCGIHandler() = default;
    ~HttpCGIHandler() override = default;

    void HandleRequest(
    const std::shared_ptr<ClientSocket>& client_socket,
    const std::shared_ptr<HttpData>& http_data
    ) override;

private:
    std::string HandlePythonCGI(const std::string& script_path, const std::shared_ptr<HttpData>& http_data);
};